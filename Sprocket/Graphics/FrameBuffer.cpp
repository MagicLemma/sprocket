#include "FrameBuffer.h"

#include <glad/glad.h>

namespace Sprocket {

FrameBuffer::FrameBuffer(int width, int height)
    : d_texture(std::make_shared<Texture>(width, height, Texture::Channels::RGBA))
    , d_depth(std::make_shared<Texture>(width, height, Texture::Channels::DEPTH))
    , d_width(width)
    , d_height(height)
{
    glCreateFramebuffers(1, &d_fbo);
    glNamedFramebufferTexture(d_fbo, GL_COLOR_ATTACHMENT0, d_texture->Id(), 0);
    glNamedFramebufferTexture(d_fbo, GL_DEPTH_ATTACHMENT, d_depth->Id(), 0);

    // Validate the framebuffer.
    if (glCheckNamedFramebufferStatus(d_fbo, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        SPKT_LOG_ERROR("Created FBO is not complete!");
    }
}

FrameBuffer::~FrameBuffer()
{
    glDeleteFramebuffers(1, &d_fbo);
}

void FrameBuffer::Bind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, d_fbo);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
}

void FrameBuffer::Unbind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::BindTexture() const
{
    glBindTexture(GL_TEXTURE_2D, d_texture->Id());
}

void FrameBuffer::UnbindTexture() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

void FrameBuffer::SetScreenSize(int width, int height)
{
    d_width = width;
    d_height = height;

    glBindTexture(GL_TEXTURE_2D, d_texture->Id());
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    
    glBindTexture(GL_TEXTURE_2D, d_depth->Id());
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    
    glBindTexture(GL_TEXTURE_2D, 0);
}

}