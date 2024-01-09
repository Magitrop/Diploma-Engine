#pragma once

#include <memory>

#include <engine/render/window/window.h>

namespace engine
{
	class IRenderPipeline;
	class WindowManager;
	class ScopedFrameFactory;
	class ScopedFrame
	{
	public:
		explicit ScopedFrame(WindowID window, const ScopedFrameFactory& factory);
		~ScopedFrame();

	private:
		WindowID m_window;
		const ScopedFrameFactory& m_factory;
	};

	class ScopedFrameFactory
	{
	public:
		explicit ScopedFrameFactory(std::shared_ptr<IRenderPipeline> renderPipeline,
									std::shared_ptr<WindowManager> windowManager);

		std::shared_ptr<IRenderPipeline> renderPipeline() const;
		std::shared_ptr<WindowManager> windowManager() const;

		ScopedFrame beginFrame(WindowID window) const;

	private:
		std::shared_ptr<IRenderPipeline> m_renderPipeline;
		std::shared_ptr<WindowManager> m_windowManager;
	};
} // namespace engine
