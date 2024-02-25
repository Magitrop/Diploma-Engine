#pragma once

#include <memory>
#include <string>

#include <engine/core/input/codes.h>
#include <engine/core/math/vector2.h>

struct GLFWwindow;
namespace engine
{
	class TimeManager;
	class InputSystem final
	{
		// friends
	private:
		friend class InputSystemAccessor;

		// members
	public:
		explicit InputSystem(std::shared_ptr<TimeManager> timeManager);

		// Returns the current position of the cursor in pixels.
		Vector2 mousePosition() const;
		// Returns the difference between the last and current cursor positions in pixels.
		Vector2 mouseMotion() const;
		// Returns the offset of the last mouse wheel scrolling.
		float mouseWheel() const;

		// Returns whether the given mouse button is held down.
		bool isMouseButtonPressed(MouseButton button) const;
		// Returns true if the given mouse button was pressed during this frame.
		bool onMouseButtonDown(MouseButton button) const;
		// Returns true if the given mouse button was released during this frame.
		bool onMouseButtonUp(MouseButton button) const;
		// Returns true if the given mouse button was clicked.
		bool onMouseClick(MouseButton button) const;

		// Returns whether the given key is held down.
		bool isKeyPressed(KeyCode keyCode) const;
		// Returns true if the given key was pressed during this frame.
		bool onKeyDown(KeyCode keyCode) const;
		// Returns true if the given key was released during this frame.
		bool onKeyUp(KeyCode keyCode) const;

		// Returns the currently applied key modifiers (Shift, Ctrl, Alt).
		KeyModifiers modifiers() const;

	private:
		class Internal;
		std::unique_ptr<Internal> m_internal;
	};
} // namespace engine