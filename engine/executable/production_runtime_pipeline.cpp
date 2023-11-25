#include "production_runtime_pipeline.h"

#include <iostream>

#include <engine/core/components/components_manager.h>
#include <engine/core/objects/objects_manager.h>
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

		m_logger = std::make_shared<Logger>(std::move(Logger(loggingDirectory)));
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

	bool ProductionRuntimePipeline::initializeGraphicAPI()
	{
		MEMORY_GUARD;

		// TODO: abstract graphic API
		DEBUG_LOG("Initializing graphic API...");
		m_graphicAPI = std::make_shared<GladGraphicAPI>(std::move(GladGraphicAPI()));
		return m_graphicAPI->initialize();
	}

	bool ProductionRuntimePipeline::initializeWindowManager()
	{
		MEMORY_GUARD;

		DEBUG_LOG("Initializing window manager...");
		m_windowManager = std::make_shared<WindowManager>(std::move(WindowManager()));
		return m_windowManager != nullptr;
	}

	bool ProductionRuntimePipeline::initializeTimeManager()
	{
		MEMORY_GUARD;

		DEBUG_LOG("Initializing time manager...");
		m_timeManager = std::make_shared<TimeManager>(std::move(TimeManager()));
		return m_timeManager != nullptr;
	}

	bool ProductionRuntimePipeline::initializeObjectsManager()
	{
		DEBUG_LOG("Initializing objects manager...");
		m_objectsManager = std::make_shared<ObjectsManager>(std::move(ObjectsManager()));
		return m_objectsManager != nullptr;
	}

	bool ProductionRuntimePipeline::initializeComponentsManager()
	{
		DEBUG_LOG("Initializing components manager...");
		m_componentsManager = std::make_shared<ComponentsManager>(std::move(ComponentsManager()));
		return m_componentsManager != nullptr;
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
