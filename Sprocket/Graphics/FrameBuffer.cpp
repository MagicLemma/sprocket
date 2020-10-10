#include "FrameBuffer.h"

#include <glad/glad.h>

namespace Sprocket {

FrameBuffer::FrameBuffer(int width, int height)
    : d_fbo(std::make_shared<FBO>())
    , d_texture(std::make_shared<Texture>(width, height, nullptr))
    , d_depthBuffer(std::make_shared<RBO>())
    , d_width(width)
    , d_height(height)
{
    glBindFramebuffer(GL_FRAMEBUFFER, d_fbo->Value());

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, d_texture->Id(), 0);

    // Add depth buffer.
    glBindRenderbuffer(GL_RENDERBUFFER, d_depthBuffer->Value());
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, d_depthBuffer->Value());

    // Validate the framebuffer.
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        SPKT_LOG_ERROR("Created FBO is not complete!");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::Bind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, d_fbo->Value());
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
    glBindTexture(GL_TEXTURE_2D, 0);

    glBindRenderbuffer(GL_RENDERBUFFER, d_depthBuffer->Value());
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

}