#include "window_manager.h"

#include <engine/core/math/vector2.h>
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

		WindowID createWindow(std::size_t width,
							  std::size_t height,
							  std::string label = "",
							  bool isResizable = true,
							  WindowID sharedContext = WindowID());
		WindowID getWindowByID(std::size_t id) const;

		void setWindowAsCurrentContext(WindowID id);
		void setWindowTitle(WindowID id, const char* title);
		void swapBuffers(WindowID id) const;

		Vector2 getWindowSize(WindowID id);

		WindowID getCurrentContextWindow() const;

	private:
		bool checkWindowID(WindowID id) const;
		GLFWwindow* getWindow(WindowID id) const;

		PersistentVector<GLFWwindow*, 8> m_createdWindows; // it is rather unlikely to have more than 8 windows at once
		WindowID m_currentContextWindowID{ static_cast<std::size_t>(-1) };
	};

	WindowID WindowManager::Internal::createWindow(std::size_t width,
												   std::size_t height,
												   std::string label /* = "" */,
												   bool isResizable /* = true */,
												   WindowID sharedContext /* = nullptr */)
	{
		MEMORY_GUARD;

		// TODO: ban non-main threads

		// setting up window parameters
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_RESIZABLE, isResizable);

		if (width == 0 || height == 0)
		{
			ERROR_LOG("Cannot create window of this size!");
			return WindowID();
		}

		// creating window
		auto sharedIt = m_createdWindows.at(sharedContext);
		GLFWwindow* shared = sharedIt != m_createdWindows.end() ? sharedIt->get() : nullptr;
		GLFWwindow* window = glfwCreateWindow(width, height, label.c_str(), nullptr, shared);
		if (!window)
		{
			ERROR_LOG("Failed to create window: internal GLFW error!");
			return WindowID();
		}

		return WindowID(m_createdWindows.push(window).getIndex());
	}

	WindowManager::Internal::~Internal()
	{
		MEMORY_GUARD;

		for (auto& window : m_createdWindows)
		{
			glfwDestroyWindow(window.get());
			TRACE_LOG("Window destructed.");
		}
	}

	engine::WindowID WindowManager::Internal::getWindowByID(std::size_t id) const
	{
		if (id < m_createdWindows.size())
		{
			return WindowID(m_createdWindows.at(id).getIndex());
		}
		WARNING_LOG("Failed to find a window with ID {}.", id);
		return WindowID();
	}

	void WindowManager::Internal::setWindowAsCurrentContext(WindowID id)
	{
		if (!checkWindowID(id)) 
			return;

		if (GLFWwindow* window = getWindow(id))
		{
			m_currentContextWindowID = id;
			glfwMakeContextCurrent(window);
		}
	}

	void WindowManager::Internal::setWindowTitle(WindowID id, const char* title)
	{
		if (!checkWindowID(id)) 
			return;

		if (GLFWwindow* window = getWindow(id))
			glfwSetWindowTitle(window, title);
	}

	void WindowManager::Internal::swapBuffers(WindowID id) const
	{
		if (!checkWindowID(id)) 
			return;

		if (GLFWwindow* window = getWindow(id))
			glfwSwapBuffers(window);
	}

	Vector2 WindowManager::Internal::getWindowSize(WindowID id)
	{
		if (!checkWindowID(id)) 
			return Vector2(0, 0);

		int x, y;
		glfwGetWindowSize(getWindow(id), &x, &y);
		return { x, y };
	}

	WindowID WindowManager::Internal::getCurrentContextWindow() const
	{
		return m_currentContextWindowID;
	}

	bool WindowManager::Internal::checkWindowID(WindowID id) const
	{
		if (!id.isValid())
		{
			ERROR_LOG("Cannot use an invalid Window ID!");
			return false;
		}
		return true;
	}

	GLFWwindow* WindowManager::Internal::getWindow(WindowID id) const
	{
		return id.isValid() ? m_createdWindows.at(id)->get() : nullptr;
	}

	WindowManager::WindowManager()
	{
		m_internal = std::make_unique<Internal>();
	}

	WindowManager::~WindowManager()
	{}

	WindowID WindowManager::createWindow(std::size_t width,
										 std::size_t height, 
										 std::string label, 
										 bool isResizable, 
										 WindowID sharedContext)
	{
		return m_internal->createWindow(width, height, label, isResizable, sharedContext);
	}

	WindowID WindowManager::getCurrentContextWindow() const
	{
		return m_internal->getCurrentContextWindow();
	}

	void WindowManager::setWindowAsCurrentContext(WindowID window)
	{
		m_internal->setWindowAsCurrentContext(window);
	}

	void WindowManager::setWindowTitle(WindowID window, const char* title)
	{
		m_internal->setWindowTitle(window, title);
	}

	void WindowManager::swapBuffers(WindowID window)
	{
		m_internal->swapBuffers(window);
	}
} // namespace engine