#include "window_manager.h"

#include <engine/debug/memory/memory_guard.h>
#include <engine/dependencies/gl/glfw/include/GLFW/glfw3.h>
#include <engine/render/window/window.h>

using WindowManager = engine::WindowManager;

const engine::Window* WindowManager::createWindow(std::size_t width,
												  std::size_t height,
												  std::string label /* = "" */,
												  bool isResizable /* = true */,
												  const Window* sharedContext /* = nullptr */)
{
	MEMORY_GUARD;

	// TODO: ban non-main threads

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
	
	return &m_createdWindows.push(std::move(window));
}

WindowManager::~WindowManager()
{
	MEMORY_GUARD;

	// TODO: begin() and end() for PersistentVector? too slow otherwise
	// for (auto& window : m_createdWindows)
	for (std::size_t i = 0; i < m_createdWindows.size(); ++i)
	{
		glfwDestroyWindow(m_createdWindows[i].m_window);
	}
}

const engine::Window* WindowManager::getWindowByID(std::size_t id) const
{
	if (id < m_createdWindows.size())
	{
        return &m_createdWindows[id];
	}
    return nullptr;
}

void WindowManager::setWindowAsCurrentContext(const Window* window)
{
	MEMORY_GUARD;

	if (window && window->isValid())
		glfwMakeContextCurrent(window->m_window);
}
