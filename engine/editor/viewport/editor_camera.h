#pragma once

#include <engine/internal/helpers/non_copyable.h>
#include <engine/internal/render/i_render_context.h>

namespace engine
{
	class EditorCamera final : NonCopyable
	{
	public:
		explicit EditorCamera(RenderContext context);

	private:
		RenderContext m_renderContext;
	};

	// A manager for all editor cameras.
	class EditorCameraManager final
	{

	private:

	};
} // namespace engine
