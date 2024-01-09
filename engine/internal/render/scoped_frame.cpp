#include "scoped_frame.h"

#include <engine/internal/render/i_render_pipeline.h>
#include <engine/render/window/window_manager_impl.h>
#include <engine/dependencies/glfw/include/GLFW/glfw3.h>

namespace engine
{
	ScopedFrameFactory::ScopedFrameFactory(std::shared_ptr<IRenderPipeline> renderPipeline,
										   std::shared_ptr<WindowManager> windowManager)
		: m_renderPipeline(renderPipeline)
		, m_windowManager(windowManager)
	{
		DEBUG_ASSERT(m_renderPipeline != nullptr);
		DEBUG_ASSERT(m_windowManager != nullptr);
	}

	std::shared_ptr<IRenderPipeline> ScopedFrameFactory::renderPipeline() const
	{
		return m_renderPipeline;
	}

	std::shared_ptr<WindowManager> ScopedFrameFactory::windowManager() const
	{
		return m_windowManager;
	}

	ScopedFrame ScopedFrameFactory::beginFrame(WindowID window) const
	{
		return ScopedFrame(window, *this);
	}

	ScopedFrame::ScopedFrame(WindowID window, const ScopedFrameFactory& factory)
		: m_window(window)
		, m_factory(factory)
	{
		m_factory.renderPipeline()->renderFrame();
	}

	ScopedFrame::~ScopedFrame()
	{
		m_factory.windowManager()->m_internal->swapBuffers(m_window);
		m_factory.renderPipeline()->clearFrame();
	}
} // namespace engine
