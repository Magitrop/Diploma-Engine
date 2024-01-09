#include "window_manager.h"

#include <engine/render/window/window_manager_impl.h>

namespace engine
{
	WindowManager::WindowManager(std::shared_ptr<InputSystem> inputSystem)
	{
		m_internal = std::make_unique<Internal>(std::move(inputSystem));
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
} // namespace engine