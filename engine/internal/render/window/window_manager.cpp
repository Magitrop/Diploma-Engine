#include "window_manager.h"

#include <engine/render/window/window.h>
#include <engine/internal/render/gl/glfw/include/GLFW/glfw3.h>

using WindowManager = engine::internal::render::WindowManager;

const WindowManager::Window& WindowManager::createWindow(std::size_t width,
														 std::size_t height,
														 std::string label /* = "" */,
														 bool isResizable /* = true */,
														 GLFWwindow* sharedContext /* = nullptr */)
{
	return Window();//m_createdWindows.emplace_back(Window::Constructor::constructShared(width, height, label, isResizable, sharedContext));
}

const WindowManager::Window& WindowManager::getWindowByID(std::size_t id) const
{
    if (id < m_createdWindows.size())
        return m_createdWindows[id];
    return Window(); // invalid window
}

void WindowManager::setWindowAsCurrentContext(const Window& window)
{
	glfwMakeContextCurrent(window.m_window);
}
