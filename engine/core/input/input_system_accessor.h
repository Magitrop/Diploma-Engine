#pragma once

#include <memory>

#include <engine/core/input/input_system.h>

struct GLFWwindow;
namespace engine
{
	class InputSystem;
	class InputSystemAccessor final
	{
	public:
		explicit InputSystemAccessor(std::shared_ptr<InputSystem> inputSystem);

		void connectCallbacksToWindow(GLFWwindow* window);
		void onFrameEnd();

		std::shared_ptr<InputSystem>& inputSystem() { return m_inputSystem; }

	private:
		std::shared_ptr<InputSystem> m_inputSystem;
	};
} // namespace engine