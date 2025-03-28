#include "input_system_impl.h"

#include <engine/core/time/time_manager.h>
#include <engine/debug/logging/debug_logger.h>
#include <engine/internal/helpers/assert.h>

namespace engine
{
	InputSystem::Internal* InputSystem::Internal::EventsHandler::internal;
	InputSystem::Internal::Internal(std::shared_ptr<TimeManager> timeManager)
		: m_timeManager(std::move(timeManager))
	{
		DEBUG_ASSERT(m_timeManager != nullptr);
		EventsHandler::internal = this;
	}

	void InputSystem::Internal::connectCallbacksToWindow(GLFWwindow* window)
	{
		glfwSetKeyCallback(window, &EventsHandler::handleKey);
		glfwSetCursorPosCallback(window, &EventsHandler::handleCursorPosition);
		glfwSetMouseButtonCallback(window, &EventsHandler::handleMouseButton);
		glfwSetScrollCallback(window, &EventsHandler::handleScroll);
	}

	Vector2 InputSystem::Internal::mousePosition() const
	{
		return m_mousePosition.thisFrame;
	}

	Vector2 InputSystem::Internal::mouseMotion() const
	{
		return m_mousePosition.thisFrame - m_mousePosition.lastFrame;
	}

	float InputSystem::Internal::mouseWheel() const
	{
		return m_mouseWheelOffset.y;
	}

	bool InputSystem::Internal::isMouseButtonPressed(MouseButton button) const
	{
		const auto& state = m_mouse[static_cast<std::uint8_t>(button)];
		return state.thisFrame;
	}

	bool InputSystem::Internal::onMouseButtonDown(MouseButton button) const
	{
		const auto& state = m_mouse[static_cast<std::uint8_t>(button)];
		return !state.lastFrame && state.thisFrame;
	}

	bool InputSystem::Internal::onMouseButtonUp(MouseButton button) const
	{
		const auto& state = m_mouse[static_cast<std::uint8_t>(button)];
		return state.lastFrame && !state.thisFrame;
	}

	bool InputSystem::Internal::onMouseClick(MouseButton button) const
	{
		const auto& state = m_mouse[static_cast<std::uint8_t>(button)];
		return state.isClicking && state.lastFrame && !state.thisFrame;
	}

	bool InputSystem::Internal::isKeyPressed(KeyCode keyCode) const
	{
		const auto& state = m_keys[static_cast<std::uint16_t>(keyCode)];
		return state.thisFrame;
	}

	bool InputSystem::Internal::onKeyDown(KeyCode keyCode) const
	{
		const auto& state = m_keys[static_cast<std::uint16_t>(keyCode)];
		return !state.lastFrame && state.thisFrame;
	}

	bool InputSystem::Internal::onKeyUp(KeyCode keyCode) const
	{
		const auto& state = m_keys[static_cast<std::uint16_t>(keyCode)];
		return state.lastFrame && !state.thisFrame;
	}

	void InputSystem::Internal::onFrameEnd()
	{
		m_mousePosition.lastFrame = m_mousePosition.thisFrame;

		for (auto& mouse : m_mouse)
		{
			mouse.lastFrame = mouse.thisFrame;
			mouse.timeSinceLastChange += m_timeManager->deltaTime();
			mouse.isClicking = false;
		}

		for (auto& key : m_keys)
			key.lastFrame = key.thisFrame;
	}

	void InputSystem::Internal::handleKey(GLFWwindow* window, int key, int scancode, int action, int mode)
	{
		if (key < 0)
			return;

		if (action == GLFW_PRESS)
			m_keys[key].thisFrame = true;
		else if (action == GLFW_RELEASE)
			m_keys[key].thisFrame = false;
	}

	void InputSystem::Internal::handleCursorPosition(GLFWwindow* window, double x, double y)
	{
		m_mousePosition.thisFrame = { x, y };
	}

	void InputSystem::Internal::handleMouseButton(GLFWwindow* window, int button, int action, int mods)
	{
		if (button < 0)
			return;

		if (action == GLFW_PRESS)
		{
			m_mouse[button].thisFrame = true;
			m_mouse[button].timeSinceLastChange = 0.0f;
		}
		else if (action == GLFW_RELEASE)
		{
			m_mouse[button].thisFrame = false;

			if (m_mouse[button].timeSinceLastChange < MOUSE_CLICK_MAX_TIME_GAP)
				m_mouse[button].isClicking = true;
			m_mouse[button].timeSinceLastChange = 0.0f;
		}
	}

	void InputSystem::Internal::handleScroll(GLFWwindow* window, double xoffset, double yoffset)
	{
		m_mouseWheelOffset = { xoffset, yoffset };
	}

	void InputSystem::Internal::EventsHandler::handleKey(GLFWwindow* window, int key, int scancode, int action, int mode)
	{
		internal->handleKey(window, key, scancode, action, mode);
	}

	void InputSystem::Internal::EventsHandler::handleCursorPosition(GLFWwindow* window, double x, double y)
	{
		internal->handleCursorPosition(window, x, y);
	}

	void InputSystem::Internal::EventsHandler::handleMouseButton(GLFWwindow* window, int button, int action, int mods)
	{
		internal->handleMouseButton(window, button, action, mods);
	}

	void InputSystem::Internal::EventsHandler::handleScroll(GLFWwindow* window, double xoffset, double yoffset)
	{
		internal->handleScroll(window, xoffset, yoffset);
	}
} // namespace engine