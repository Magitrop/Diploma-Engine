#include "window.h"

#include <engine/internal/render/gl/glfw/include/GLFW/glfw3.h>

using Window = engine::render::Window;

Window::Window(
	std::size_t width, 
	std::size_t height, 
	std::string label /* = "" */, 
	bool isResizable /* = true */, 
	GLFWwindow* sharedContext /* = nullptr */) 
	: m_width(width)
	, m_height(height)
	, m_isResizable(isResizable)
	, m_isValid(true)
{
	// setting up window parameters
	glfwWindowHint(GLFW_RESIZABLE, m_isResizable);

	// creating window
	m_window = glfwCreateWindow(width, height, label.c_str(), nullptr, sharedContext);
	if (!m_window)
	{
		glfwTerminate();
		Logger::instance().log("Failed to create window!", Logger::Level::Fatal);
	}
}

Window::Window()
	: m_width(0)
	, m_height(0)
	, m_isResizable(false)
	, m_isValid(false)
	, m_window(nullptr) 
{}

Window::~Window()
{
	glfwDestroyWindow(m_window);
}

std::size_t Window::width() const
{
	return m_width;
}

std::size_t Window::height() const
{
	return m_height;
}

bool Window::isResizable() const
{
	return m_isResizable;
}

bool Window::isValid() const
{
	return m_isValid;
}
