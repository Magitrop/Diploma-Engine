#include "window_manager.h"

#include <engine/debug/memory/memory_guard.h>
#include <engine/dependencies/gl/glfw/include/GLFW/glfw3.h>
#include <engine/render/window/window.h>

namespace engine
{
	const Window* WindowManager::createWindow(std::size_t width,
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
			ERROR_LOG("Failed to create window: internal GLFW error!");
			return nullptr;
		}

		Window window = Window(width, height, label, isResizable, glfwWindow);
		if (!window.isValid())
		{
			ERROR_LOG("Failed to create window!");
			glfwDestroyWindow(glfwWindow);
			return nullptr;
		}

		return &m_createdWindows.push(std::move(window));
	}

	WindowManager::~WindowManager()
	{
		MEMORY_GUARD;

		for (auto& page : m_createdWindows)
			for (auto& window : *page)
				glfwDestroyWindow(window.get().m_window);
	}

	const engine::Window* WindowManager::getWindowByID(std::size_t id) const
	{
		/*if (id < m_createdWindows.size())
		{
			return &m_createdWindows[id];
		}*/
		return nullptr;
	}

	void WindowManager::setWindowAsCurrentContext(const Window* window)
	{
		MEMORY_GUARD;

		if (window && window->isValid())
			glfwMakeContextCurrent(window->m_window);
	}

	void WindowManager::setWindowTitle(const Window* window, const char* title)
	{
		MEMORY_GUARD;

		if (window && window->isValid())
			glfwSetWindowTitle(window->m_window, title);
	}

	void WindowManager::swapBuffers(const Window* window)
	{
		MEMORY_GUARD;

		if (window && window->isValid())
			glfwSwapBuffers(window->m_window);
	}
} // namespace engine