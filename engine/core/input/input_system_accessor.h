#pragma once

#include <memory>

struct GLFWwindow;
namespace engine
{
	class InputSystem;
	class InputSystemAccessor final
	{
	public:
		explicit InputSystemAccessor(std::shared_ptr<InputSystem> inputSystem);

		void connectCallbacksToWindow(GLFWwindow* window);
		void onFrameBegin();

	private:
		std::shared_ptr<InputSystem> m_inputSystem;
	};
} // namespace engine