#include "glad_framebuffer.h"

#include <engine/debug/logging/debug_logger.h>

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
        glGenTextures(1, &m_depthTexture);

        glBindTexture(GL_TEXTURE_2D, m_opaqueTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_SHORT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glBindTexture(GL_TEXTURE_2D, m_depthTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_width, m_height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_SHORT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        // Attaching the texture to the framebuffer
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_opaqueTexture, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthTexture, 0);

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

    float GladFramebuffer::aspectRatio() const
    {
        return static_cast<float>(m_width) / static_cast<float>(m_height);
    }

    ImTextureID GladFramebuffer::textureID() const
    {
        // suppressing the C4312 warning
        return ImTextureID(static_cast<std::size_t>(m_opaqueTexture));
    }

    bool GladFramebuffer::setSize(std::uint32_t x, std::uint32_t y)
    {
        if (m_width == x && m_height == y)
            return false;

        m_width = x;
        m_height = y;

        glBindTexture(GL_TEXTURE_2D, m_opaqueTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

        glBindTexture(GL_TEXTURE_2D, m_depthTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_width, m_height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_SHORT, NULL);

        glBindTexture(GL_TEXTURE_2D, 0);

        return true;
    }

    GLuint GladFramebuffer::FBO() const
    {
        return m_FBO;
    }

    ScopedFramebuffer GladFramebuffer::useFramebuffer()
    {
        return ScopedFramebuffer(m_FBO);
    }

    PixelInfo GladFramebuffer::readPixel(std::uint32_t x, std::uint32_t y)
    {
        ScopedFramebuffer scopedFramebuffer = useFramebuffer();

        PixelInfo info{};
        glReadPixels(x, y, 1, 1, GL_RGBA, GL_UNSIGNED_SHORT, &info.color);
        glReadPixels(x, y, 1, 1, GL_DEPTH_COMPONENT, GL_UNSIGNED_SHORT, &info.depth);
        return info;
    }
} // namespace engine
