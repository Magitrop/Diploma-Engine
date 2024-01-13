#include "glad_framebuffer.h"

namespace engine
{
    GladFramebuffer::GladFramebuffer(std::uint32_t width, std::uint32_t height)
        : m_width(width)
        , m_height(height)
    {
        // Creating a framebuffer
        glGenFramebuffers(1, &m_FBO);
        glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

        // Creating textures
        glGenTextures(1, &m_opaqueTexture);
        glBindTexture(GL_TEXTURE_2D, m_opaqueTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Attaching the texture to the framebuffer
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_opaqueTexture, 0);

        glBindTexture(GL_TEXTURE_2D, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    GladFramebuffer::~GladFramebuffer()
    {
        glDeleteFramebuffers(1, &m_FBO);
        glDeleteTextures(1, &m_opaqueTexture);
    }

    std::uint32_t GladFramebuffer::width() const
    {
        return m_width;
    }

    std::uint32_t GladFramebuffer::height() const
    {
        return m_height;
    }

    ImTextureID GladFramebuffer::textureID() const
    {
        // suppressing the C4312 warning
        return ImTextureID(static_cast<std::size_t>(m_opaqueTexture));
    }

    GLuint GladFramebuffer::FBO() const
    {
        return m_FBO;
    }

    GladFramebuffer::ScopedFramebuffer GladFramebuffer::useFramebuffer()
    {
        return ScopedFramebuffer(m_FBO);
    }
} // namespace engine
