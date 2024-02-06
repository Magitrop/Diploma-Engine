#pragma once

#if IS_EDITOR
#include <engine/editor/viewport/editor_framebuffer.h>
#else
#include <engine/internal/render/i_framebuffer.h>
#endif // #if IS_EDITOR

#include <engine/internal/helpers/non_copyable.h>
#include <engine/internal/render/glad/glad_graphic_api.h>

namespace engine
{
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

	class GladFramebuffer final
#if IS_EDITOR
		: public EditorFramebuffer
#else
		: public IFramebuffer
#endif // #if IS_EDITOR
	{
	public:
		explicit GladFramebuffer(std::uint32_t width, std::uint32_t height);
		virtual ~GladFramebuffer() override;

		virtual std::uint32_t width() const override;
		virtual std::uint32_t height() const override;
		virtual float aspectRatio() const override;
#if IS_EDITOR
		virtual ImTextureID textureID() const override;
		virtual bool setSize(std::uint32_t x, std::uint32_t y) override;
#endif // #if IS_EDITOR
		GLuint FBO() const;

		[[nodiscard]] ScopedFramebuffer useFramebuffer();

		virtual PixelInfo readPixel(std::uint32_t x, std::uint32_t y) override;

	private:
		std::uint32_t m_width;
		std::uint32_t m_height;
		GLuint m_FBO; // framebuffer object
		GLuint m_opaqueTexture; // an opaque (color) texture for the framebuffer
		GLuint m_depthTexture; // a depth texture for the framebuffer
	};
} // namespace engine
