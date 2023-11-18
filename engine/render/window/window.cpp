#include "window.h"

#include <engine/dependencies/gl/glfw/include/GLFW/glfw3.h>

namespace engine
{
	Window::Window(
		std::size_t width,
		std::size_t height,
		std::string label /* = "" */,
		bool isResizable /* = true */,
		GLFWwindow* window /* = nullptr */)
		: m_width(width)
		, m_height(height)
		, m_isResizable(isResizable)
		, m_window(window)
		, m_isValid(true)
	{
		if (width == 0 || height == 0 || window == nullptr)
		{
			m_isValid = false;
			return;
		}
	}

	Window::Window()
		: m_width(0)
		, m_height(0)
		, m_isResizable(false)
		, m_isValid(false)
		, m_window(nullptr)
	{}

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
} // namespace engine