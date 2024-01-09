#pragma once

#include <memory>
#include <vector>

#include <engine/render/window/window.h>

namespace engine
{
	class InputSystem;
	// A factory and container for Window objects.
	class WindowManager final
	{
		// friends
	private:
		friend class ProductionRuntimePipeline;
		friend class EditorRuntimePipeline;
		friend class ScopedFrame;

		// members
	private:
		explicit WindowManager(std::shared_ptr<InputSystem> inputSystem);

	public:
		~WindowManager();

		WindowID createWindow(std::size_t width,
							  std::size_t height,
							  std::string label = "",
							  bool isResizable = true,
							  WindowID sharedContext = WindowID());
		WindowID getCurrentContextWindow() const;

		void setWindowAsCurrentContext(WindowID window);
		void setWindowTitle(WindowID window, const char* title);

	private:
		class Internal;
		std::unique_ptr<Internal> m_internal;
	};
} // namespace engine