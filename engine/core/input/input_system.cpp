#include "input_system.h"

#include <engine/core/input/input_system_impl.h>
#include <engine/internal/core/constants/runtime_constants.h>

namespace engine
{
	InputSystem::InputSystem()
	{
		m_internal = std::make_unique<Internal>();
	}

	Vector2 InputSystem::mousePosition() const
	{
		return m_internal->mousePosition();
	}

	Vector2 InputSystem::mouseMotion() const
	{
		return m_internal->mouseMotion();
	}

	float InputSystem::mouseWheel() const
	{
		return m_internal->mouseWheel();
	}

	bool InputSystem::isMouseButtonPressed(MouseButton button) const
	{
		return m_internal->isMouseButtonPressed(button);
	}

	bool InputSystem::onMouseButtonDown(MouseButton button) const
	{
		return m_internal->onMouseButtonDown(button);
	}

	bool InputSystem::onMouseButtonUp(MouseButton button) const
	{
		return m_internal->onMouseButtonUp(button);
	}

	bool InputSystem::isKeyPressed(KeyCode keyCode) const
	{
		return m_internal->isKeyPressed(keyCode);
	}

	bool InputSystem::onKeyDown(KeyCode keyCode) const
	{
		return m_internal->onKeyDown(keyCode);
	}

	bool InputSystem::onKeyUp(KeyCode keyCode) const
	{
		return m_internal->onKeyUp(keyCode);
	}

	InputSystem::KeyModifiers InputSystem::modifiers() const
	{
		return m_internal->modifiers();
	}
} // namespace engine