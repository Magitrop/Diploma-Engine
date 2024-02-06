#include "production_runtime_pipeline.h"

#include <iostream>

#include <engine/internal/render/glad/glad_graphic_api.h>
#include <engine/internal/render/glad/glad_mesh_renderer_component.h>
#include <engine/internal/render/glad/glad_render_pipeline.h>
#include <engine/internal/render/glad/glad_resource_manager.h>

#include <engine/core/components/component.h>
#include <engine/core/components/component_registrar.h>
#include <engine/core/components/renderer_component.h>
#include <engine/core/components/transform_component.h>
#include <engine/core/entity/entity_manager.h>
#include <engine/core/input/input_system_impl.h>
#include <engine/core/input/input_system_accessor.h>
#include <engine/core/resources/resource_registrar.h>
#include <engine/core/time/time_manager.h>

#include <engine/debug/logging/debug_logger.h>
#include <engine/debug/memory/memory_guard.h>

#include <engine/executable/runtime.h>

#include <engine/render/window/window_manager.h>

namespace engine
{
	bool ProductionRuntimePipeline::initialize()
	{
		MEMORY_GUARD;

		return true;
	}

	void ProductionRuntimePipeline::finalize()
	{
	}

	void ProductionRuntimePipeline::run()
	{
		MEMORY_GUARD;
	}

#if USE_LOGGER
	bool ProductionRuntimePipeline::initializeLogger()
	{
		MEMORY_GUARD;

		std::filesystem::path loggingDirectory = std::filesystem::current_path();
		loggingDirectory += "\\logs\\";

		m_logger = std::shared_ptr<Logger>(new Logger(loggingDirectory));
		m_logger->setMinimumLevel(Logger::Level::Debug);
		m_logger->useFileLogging(false);

		// setting the logger's static instance to be able to use its functions anywhere
		Logger::m_instance = m_logger;

		return m_logger != nullptr;
	}
#endif // #if USE_LOGGER

	bool ProductionRuntimePipeline::initializeGLFW()
	{
		MEMORY_GUARD;

		DEBUG_LOG("Initializing GLFW...");
		if (!glfwInit())
		{
			FATAL_LOG("GLFW initialization failed. Further initialization is ceased.");
			return false;
		}

		glfwSetErrorCallback(
			[](int code, const char* message)
			{
				FATAL_LOG("GLFW error (code: {}): {}.", code, message);
			});

		return true;
	}

	bool ProductionRuntimePipeline::initializeInputSystem()
	{
		MEMORY_GUARD;

		DEBUG_LOG("Initializing input system...");
		m_inputSystem = std::shared_ptr<InputSystem>(new InputSystem());
		m_inputSystemAccessor = std::shared_ptr<InputSystemAccessor>(new InputSystemAccessor(m_inputSystem));
		return m_inputSystem != nullptr && m_inputSystemAccessor != nullptr;
	}

	bool ProductionRuntimePipeline::initializeWindowManager()
	{
		MEMORY_GUARD;

		DEBUG_LOG("Initializing window manager...");
		m_windowManager = std::shared_ptr<WindowManager>(new WindowManager(m_inputSystemAccessor));
		return m_windowManager != nullptr;
	}

	bool ProductionRuntimePipeline::initializeGraphicAPI()
	{
		MEMORY_GUARD;

		// TODO: abstract graphic API
		DEBUG_LOG("Initializing graphic API...");
		m_graphicAPI = std::shared_ptr<IGraphicAPI>(new GladGraphicAPI());
		return m_graphicAPI->initialize();
	}

	bool ProductionRuntimePipeline::initializeTimeManager()
	{
		MEMORY_GUARD;

		DEBUG_LOG("Initializing time manager...");
		m_timeManager = std::shared_ptr<TimeManager>(new TimeManager());
		return m_timeManager != nullptr;
	}

	bool ProductionRuntimePipeline::initializeComponentRegistrar()
	{
		MEMORY_GUARD;

		DEBUG_LOG("Initializing component registrar...");
		m_componentRegistrar = std::shared_ptr<ComponentRegistrar>(new ComponentRegistrar());
		return m_componentRegistrar != nullptr;
	}

	bool ProductionRuntimePipeline::initializeEntityManager()
	{
		MEMORY_GUARD;

		DEBUG_LOG("Initializing entity manager...");

		DEBUG_ASSERT(m_componentRegistrar != nullptr);
		m_entityManager = std::shared_ptr<EntityManager>(new EntityManager(m_componentRegistrar));
		return m_entityManager != nullptr;
	}

	bool ProductionRuntimePipeline::initializeResourceManager()
	{
		MEMORY_GUARD;

		DEBUG_LOG("Initializing resource manager...");
		// TODO: abstract graphic API
		m_resourceManager = std::shared_ptr<ResourceManager>(new GladResourceManager());
		return m_resourceManager != nullptr;
	}

	bool ProductionRuntimePipeline::registerBuiltinComponents()
	{
		MEMORY_GUARD;

		DEBUG_LOG("Registering built-in components...");

		DEBUG_ASSERT(m_resourceManager != nullptr);
		// TODO: remake with a proper macro (see serialization_helper.h)
		m_componentRegistrar->registerComponent<Transform>(std::shared_ptr<Transform>(new Transform()), "Transform");
		// TODO: abstract graphic API
		m_componentRegistrar->registerComponent<MeshRenderer>(std::shared_ptr<GladMeshRenderer>(new GladMeshRenderer(m_resourceManager)), "MeshRenderer");

		return true;
	}

	bool ProductionRuntimePipeline::initializeRenderPipeline()
	{
		MEMORY_GUARD;

		DEBUG_LOG("Initializing render pipeline...");
		// TODO: abstract graphic API
		DEBUG_ASSERT(m_entityManager != nullptr);
		DEBUG_ASSERT(m_resourceManager != nullptr);
		m_renderPipeline = std::shared_ptr<IRenderPipeline>(
			new GladRenderPipeline(m_entityManager,
								   m_resourceManager)
		);
		return m_renderPipeline != nullptr;
	}

	bool ProductionRuntimePipeline::registerBuiltinResources()
	{
		MEMORY_GUARD;

		auto resourceRegistrar = std::dynamic_pointer_cast<ResourceRegistrar>(m_resourceManager);
		DEBUG_ASSERT(resourceRegistrar != nullptr);

		DEBUG_LOG("Registering built-in resources...");

		resourceRegistrar->registerBuiltinShaders();
		resourceRegistrar->registerBuiltinMaterials();
		return true;
	}

	void ProductionRuntimePipeline::finalizeGLFW()
	{
		DEBUG_LOG("Finalizing GLFW...");
		glfwTerminate();
	}

	void ProductionRuntimePipeline::finalizeGraphicAPI()
	{
		DEBUG_LOG("Finalizing graphic API...");
		m_graphicAPI->finalize();
	}

	ScopedTime ProductionRuntimePipeline::startDeltaTimer()
	{
		return ScopedTime(m_timeManager.get());
	}
} // namespace engine
