#include "window.h"

#include <engine/internal/render/gl/glfw/include/GLFW/glfw3.h>

engine::render::Window::Window(
	size_t width, 
	size_t height, 
	std::string label /* = "" */, 
	bool isResizable /* = true */, 
	GLFWwindow* sharedContext /* = nullptr */)
{
	m_width = width;
	m_height = height;
	m_isResizable = isResizable;

	// setting window parameters
	glfwWindowHint(GLFW_RESIZABLE, m_isResizable);

	// creating window
	m_window = glfwCreateWindow(width, height, label.c_str(), nullptr, sharedContext);
	if (!m_window)
	{
		Logger::instance().log("Failed to create window!", Logger::Level::Fatal);
		glfwTerminate();
	}
}

size_t engine::render::Window::width() const
{
	return m_width;
}

size_t engine::render::Window::height() const
{
	return m_height;
}

bool engine::render::Window::isResizable() const
{
	return m_isResizable;
}
