#pragma once

#if IS_EDITOR
#include <engine/editor/viewport/editor_framebuffer.h>
#endif // #if IS_EDITOR

#include <engine/internal/helpers/non_copyable.h>
#include <engine/internal/render/glad/glad_graphic_api.h>
#include <engine/internal/render/glad/glad_render_pipeline.h>

namespace engine
{
	class GladFramebuffer final
#if IS_EDITOR
		: public EditorFramebuffer
#else
		: public IFramebuffer
#endif // #if IS_EDITOR
	{
	private:
		struct ScopedFramebuffer
		{
			ScopedFramebuffer(GLuint fbo) 
			{
				glBindFramebuffer(GL_FRAMEBUFFER, fbo);
			}

			~ScopedFramebuffer()
			{
				glBindFramebuffer(GL_FRAMEBUFFER, 0);
			}
		};

	public:
		explicit GladFramebuffer(std::uint32_t width, std::uint32_t height);
		virtual ~GladFramebuffer() override;

		virtual std::uint32_t width() const override;
		virtual std::uint32_t height() const override;
#if IS_EDITOR
		virtual ImTextureID textureID() const override;
#endif // #if IS_EDITOR
		GLuint FBO() const;

		ScopedFramebuffer useFramebuffer();

	private:
		std::uint32_t m_width;
		std::uint32_t m_height;
		GLuint m_FBO; // framebuffer object
		GLuint m_opaqueTexture; // an opaque (color) texture for the framebuffer
		// TODO: depth texture via renderbuffer
	};
} // namespace engine
