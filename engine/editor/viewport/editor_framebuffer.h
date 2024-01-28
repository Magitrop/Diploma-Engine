#pragma once

#include <engine/dependencies/imgui/imgui.h>
#include <engine/internal/render/i_render_pipeline.h>

namespace engine
{
	class EditorFramebuffer : public IFramebuffer
	{
	public:
		virtual ImTextureID textureID() const = 0;

		// Returns whether the size of the framebuffer has changed.
		virtual bool setSize(std::uint32_t x, std::uint32_t y) = 0;
	};
} // namespace engine