#include "window_manager.h"

#include <engine/render/window/window.h>
#include <engine/internal/render/gl/glfw/include/GLFW/glfw3.h>

using WindowManager = engine::internal::render::WindowManager;

const WindowManager::Window* WindowManager::createWindow(std::size_t width,
														 std::size_t height,
														 std::string label /* = "" */,
														 bool isResizable /* = true */,
														 const Window* sharedContext /* = nullptr */)
{
	// setting up window parameters
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, isResizable);

	// creating window
	GLFWwindow* glfwWindow = glfwCreateWindow(width, height, label.c_str(), nullptr, sharedContext ? sharedContext->m_window : nullptr);
	if (!glfwWindow)
	{
		//Logger::instance().log("Failed to create window!", Logger::Level::Fatal);
		return nullptr;
	}

	Window window = Window(width, height, label, isResizable, glfwWindow);
	if (!window.isValid())
	{
		//Logger::instance().log("Failed to create window!", Logger::Level::Fatal);
		glfwDestroyWindow(glfwWindow);
		return nullptr;
	}
	
	return &m_createdWindows.emplace_back(window);
}

WindowManager::~WindowManager()
{
	for (auto& window : m_createdWindows)
	{
		glfwDestroyWindow(window.m_window);
	}
}

const WindowManager::Window* WindowManager::getWindowByID(std::size_t id) const
{
	if (id < m_createdWindows.size())
	{
		auto it = m_createdWindows.begin();
		std::advance(it, id);
        return &*it;
	}
    return nullptr;
}

void WindowManager::setWindowAsCurrentContext(const Window* window)
{
	if (window && window->isValid())
		glfwMakeContextCurrent(window->m_window);
}
