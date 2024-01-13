#pragma once

#include <engine/dependencies/imgui/imgui.h>
#include <engine/internal/render/i_render_pipeline.h>

namespace engine
{
	class EditorFramebuffer : public IFramebuffer
	{
	public:
		virtual ImTextureID textureID() const = 0;
	};
} // namespace engine