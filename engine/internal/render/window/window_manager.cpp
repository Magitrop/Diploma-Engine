#include "window_manager.h"

#include <engine/debug/memory/memory_guard.h>
#include <engine/internal/helpers/persistent_vector.h>
#include <engine/render/window/window.h>

#include <engine/dependencies/gl/glfw/include/GLFW/glfw3.h>

namespace engine
{
	class WindowManager::Internal final
	{
	public:
		Internal() = default;
		~Internal();

		const Window* createWindow(std::size_t width,
								   std::size_t height,
								   std::string label = "",
								   bool isResizable = true,
								   const Window* sharedContext = nullptr);
		const Window* getWindowByID(std::size_t id) const;

		void setWindowAsCurrentContext(const Window* window);
		void setWindowTitle(const Window* window, const char* title);
		void swapBuffers(const Window* window);

	private:
		PersistentVector<Window, 8> m_createdWindows; // it is rather unlikely to have more than 8 windows at once
	};

	const Window* WindowManager::Internal::createWindow(std::size_t width,
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

	WindowManager::Internal::~Internal()
	{
		MEMORY_GUARD;

		for (auto& window : m_createdWindows)
		{
			glfwDestroyWindow(window.m_window);
			TRACE_LOG("Window destructed.");
		}
	}

	const engine::Window* WindowManager::Internal::getWindowByID(std::size_t id) const
	{
		if (id < m_createdWindows.size())
		{
			return &*m_createdWindows.at(id);
		}
		WARNING_LOG("Failed to find a window with ID {}.", id);
		return nullptr;
	}

	void WindowManager::Internal::setWindowAsCurrentContext(const Window* window)
	{
		MEMORY_GUARD;

		if (window && window->isValid())
			glfwMakeContextCurrent(window->m_window);
	}

	void WindowManager::Internal::setWindowTitle(const Window* window, const char* title)
	{
		MEMORY_GUARD;

		if (window && window->isValid())
			glfwSetWindowTitle(window->m_window, title);
	}

	void WindowManager::Internal::swapBuffers(const Window* window)
	{
		MEMORY_GUARD;

		if (window && window->isValid())
			glfwSwapBuffers(window->m_window);
	}

	WindowManager::WindowManager()
	{
		m_internal = std::make_unique<Internal>();
	}

	WindowManager::~WindowManager()
	{}

	const Window* WindowManager::createWindow(std::size_t width,
											  std::size_t height, 
											  std::string label, 
											  bool isResizable, 
											  const Window* sharedContext)
	{
		return m_internal->createWindow(width, height, label, isResizable, sharedContext);
	}

	const Window* WindowManager::getWindowByID(std::size_t id) const
	{
		return m_internal->getWindowByID(id);
	}

	void WindowManager::setWindowAsCurrentContext(const Window* window)
	{
		m_internal->setWindowAsCurrentContext(window);
	}

	void WindowManager::setWindowTitle(const Window* window, const char* title)
	{
		m_internal->setWindowTitle(window, title);
	}

	void WindowManager::swapBuffers(const Window* window)
	{
		m_internal->swapBuffers(window);
	}
} // namespace engine