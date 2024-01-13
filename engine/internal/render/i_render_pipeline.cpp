#include "i_render_pipeline.h"

namespace engine
{
	IFramebuffer::~IFramebuffer() {}

	EditorCamera IRenderPipeline::createEditorCamera(std::uint32_t initialViewportWidth,
													 std::uint32_t initialViewportHeight)
	{
		DEBUG_ASSERT(m_editorViewports != nullptr);

		std::shared_ptr<IFramebuffer> framebuffer = createFramebuffer(initialViewportWidth,
																	  initialViewportHeight);
		return m_editorViewports->createCamera(framebuffer);
	}
} // namespace engine
