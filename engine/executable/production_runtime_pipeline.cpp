#include "production_runtime_pipeline.h"

#include <iostream>

#include <engine/core/components/component.h>
#include <engine/core/components/component_registrar.h>
#include <engine/core/entity/entity_manager.h>
#include <engine/core/time/time_manager.h>

#include <engine/debug/logging/debug_logger.h>
#include <engine/debug/memory/memory_guard.h>

#include <engine/executable/runtime.h>

#include <engine/internal/render/graphic_api/glad_graphic_api.h>
#include <engine/internal/render/window/window_manager.h>

#include <engine/dependencies/gl/glfw/include/GLFW/glfw3.h>

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

	bool ProductionRuntimePipeline::initializeWindowManager()
	{
		MEMORY_GUARD;

		DEBUG_LOG("Initializing window manager...");
		m_windowManager = std::shared_ptr<WindowManager>(new WindowManager());
		return m_windowManager != nullptr;
	}

	bool ProductionRuntimePipeline::initializeGraphicAPI()
	{
		MEMORY_GUARD;

		// TODO: abstract graphic API
		DEBUG_LOG("Initializing graphic API...");
		m_graphicAPI = std::shared_ptr<GladGraphicAPI>(new GladGraphicAPI());
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
		m_entityManager = std::shared_ptr<EntityManager>(new EntityManager(m_componentRegistrar));
		return m_entityManager != nullptr;
	}

	bool ProductionRuntimePipeline::registerBuiltinComponents()
	{
		MEMORY_GUARD;

		DEBUG_LOG("Registering built-in components...");

		//m_componentRegistrar->registerComponent<Comp>(std::shared_ptr<Comp>(new Comp()));

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
