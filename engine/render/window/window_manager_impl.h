#pragma once

#include <engine/core/math/vector2.h>
#include <engine/internal/helpers/persistent_vector.h>
#include <engine/render/window/window_manager.h>

struct GLFWwindow;
namespace engine
{
	class WindowManager::Internal final
	{
	public:
		Internal(std::shared_ptr<InputSystem> inputSystem);
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

		GLFWwindow* getWindow(WindowID id) const;
		GLFWwindow* getCurrentContextGLFWwindow() const;
	private:
		bool checkWindowID(WindowID id) const;

		PersistentVector<GLFWwindow*, 8> m_createdWindows; // it is rather unlikely to have more than 8 windows at once
		WindowID m_currentContextWindowID{ static_cast<std::size_t>(-1) };

		std::shared_ptr<InputSystem> m_inputSystem;
	};
} // namespace engine