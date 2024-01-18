#include "input_system_accessor.h"

#include <engine/core/input/input_system_impl.h>

namespace engine
{
	InputSystemAccessor::InputSystemAccessor(std::shared_ptr<InputSystem> inputSystem)
		: m_inputSystem(inputSystem)
	{}

	void InputSystemAccessor::connectCallbacksToWindow(GLFWwindow* window)
	{
		m_inputSystem->m_internal->connectCallbacksToWindow(window);
	}

	void InputSystemAccessor::onFrameBegin()
	{
		m_inputSystem->m_internal->onFrameBegin();
	}
} // namespace engine
