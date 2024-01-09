#pragma once

#include <engine/core/input/input_system.h>
#include <engine/dependencies/glfw/include/GLFW/glfw3.h>

namespace engine
{
	class InputSystem::Internal final
	{
	public:
		explicit Internal();

		void connectCallbacksToWindow(GLFWwindow* window);

		Vector2 mousePosition() const;
		Vector2 mouseMotion() const;
		float mouseWheel() const;

		bool isMouseButtonPressed(MouseButton button) const;
		bool onMouseButtonDown(MouseButton button) const;
		bool onMouseButtonUp(MouseButton button) const;

		bool isKeyPressed(KeyCode keyCode) const;
		bool onKeyDown(KeyCode keyCode) const;
		bool onKeyUp(KeyCode keyCode) const;

		KeyModifiers modifiers() const;

	private:
		void handleKey(GLFWwindow* window, int key, int scancode, int action, int mode);
		void handleCursorPosition(GLFWwindow* window, double x, double y);
		void handleMouseButton(GLFWwindow* window, int button, int action, int mods);
		void handleScroll(GLFWwindow* window, double xoffset, double yoffset);

		template<typename T>
		struct InputState
		{
			T lastFrame{};
			T thisFrame{};
		};

		InputState<bool> m_mouse[GLFW_MOUSE_BUTTON_LAST + 1];
		InputState<bool> m_keys[GLFW_KEY_LAST + 1];
		InputState<Vector2> m_mousePosition;
		Vector2 m_mouseWheelOffset = Vector2(0, 0);
		KeyModifiers m_modifiers = KeyModifiers::None;

		struct EventsHandler
		{
			static void handleKey(GLFWwindow* window, int key, int scancode, int action, int mode);
			static void handleCursorPosition(GLFWwindow* window, double x, double y);
			static void handleMouseButton(GLFWwindow* window, int button, int action, int mods);
			static void handleScroll(GLFWwindow* window, double xoffset, double yoffset);

			static Internal* internal;
		};
	};
} // namespace engine
