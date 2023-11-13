#include "production_runtime_pipeline.h"

#include <engine/internal/render/gl/glad/include/glad/glad.h>
#include <engine/internal/render/gl/glfw/include/GLFW/glfw3.h>
#include <engine/internal/render/window/window_manager.h>
#include <engine/executable/runtime.h>

#include <iostream>

using ProductionRuntimePipeline = engine::executable::ProductionRuntimePipeline;

void ProductionRuntimePipeline::initialize()
{
}

void ProductionRuntimePipeline::run()
{
}

void ProductionRuntimePipeline::initializeGLFW()
{
	if (!glfwInit())
	{
		std::cout << "glfwInit failed" << std::endl;
	}
}

void ProductionRuntimePipeline::initializeGraphicAPI()
{
	if (!gladLoadGL())
	{
		std::cout << "gladLoadGL failed" << std::endl;
		//return;
	}
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		//return;
	}
	std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
}

void ProductionRuntimePipeline::initializeWindowManager()
{
	m_windowManager = std::make_shared<WindowManager>(WindowManager());
}