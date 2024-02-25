#include "input_system.h"

#include <engine/core/input/input_system_impl.h>
#include <engine/core/time/time_manager.h>
#include <engine/internal/core/constants/runtime_constants.h>

namespace engine
{
	InputSystem::InputSystem(std::shared_ptr<TimeManager> timeManager)
	{
		m_internal = std::make_unique<Internal>(std::move(timeManager));
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

	bool InputSystem::onMouseClick(MouseButton button) const
	{
		return m_internal->onMouseClick(button);
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

	KeyModifiers InputSystem::modifiers() const
	{
		return m_internal->modifiers();
	}
} // namespace engine