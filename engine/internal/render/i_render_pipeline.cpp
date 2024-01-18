#include "i_render_pipeline.h"

namespace engine
{
	IFramebuffer::~IFramebuffer() {}

	EditorViewportWindow IRenderPipeline::createEditorViewport(std::uint32_t initialViewportWidth,
															   std::uint32_t initialViewportHeight)
	{
		DEBUG_ASSERT(m_editorViewports != nullptr);
		DEBUG_ASSERT(m_editorViewports != nullptr);

		std::shared_ptr<IFramebuffer> framebuffer = createFramebuffer(initialViewportWidth,
																	  initialViewportHeight);
		return m_editorViewports->createViewport(framebuffer, m_inputSystem);
	}
} // namespace engine
