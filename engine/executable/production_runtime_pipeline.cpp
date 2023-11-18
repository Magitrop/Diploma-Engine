#include "production_runtime_pipeline.h"

#include <engine/debug/memory/memory_guard.h>
#include <engine/dependencies/gl/glfw/include/GLFW/glfw3.h>
#include <engine/internal/render/graphic_api/glad_graphic_api.h>
#include <engine/internal/render/window/window_manager.h>
#include <engine/executable/runtime.h>

#include <iostream>

namespace engine
{
	void ProductionRuntimePipeline::initialize()
	{
		MEMORY_GUARD;
	}

	void ProductionRuntimePipeline::finalize()
	{
	}

	void ProductionRuntimePipeline::run()
	{
	}

	void ProductionRuntimePipeline::initializeGLFW()
	{
		MEMORY_GUARD;

		if (!glfwInit())
		{
			std::cout << "glfwInit failed" << std::endl;
		}
	}

	void ProductionRuntimePipeline::initializeGraphicAPI()
	{
		MEMORY_GUARD;

		// TODO: abstract graphic API
		m_graphicAPI = std::make_shared<GladGraphicAPI>(GladGraphicAPI());
		m_graphicAPI->initialize();
	}

	void ProductionRuntimePipeline::initializeWindowManager()
	{
		MEMORY_GUARD;

		m_windowManager = std::make_shared<WindowManager>(WindowManager());
	}

	void ProductionRuntimePipeline::finalizeGLFW()
	{
		glfwTerminate();
	}

	void ProductionRuntimePipeline::finalizeGraphicAPI()
	{
		m_graphicAPI->finalize();
	}
} // namespace engine
