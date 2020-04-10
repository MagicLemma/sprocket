#include "FrameBuffer.h"

#include <glad/glad.h>

namespace Sprocket {

FrameBuffer::FrameBuffer(int width, int height, float scale)
    : d_fbo(std::make_shared<FBO>())
    , d_texture(std::make_shared<TEX>())
    , d_depthBuffer(std::make_shared<RBO>())
    , d_width(width)
    , d_height(height)
    , d_scale(scale)
{
    glBindFramebuffer(GL_FRAMEBUFFER, d_fbo->value());
    glBindTexture(GL_TEXTURE_2D, d_texture->value());
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width * scale, height * scale, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, d_texture->value(), 0);

    // Add depth buffer.
    glBindRenderbuffer(GL_RENDERBUFFER, d_depthBuffer->value());
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width * scale, height * scale);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, d_depthBuffer->value());

    // Validate the framebuffer.
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        SPKT_LOG_ERROR("Created FBO is not complete!");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::bind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, d_fbo->value());
    glViewport(0, 0, d_width * d_scale, d_height * d_scale);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
}

void FrameBuffer::unbind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, d_width, d_height);
}

void FrameBuffer::bindTexture() const
{
    glBindTexture(GL_TEXTURE_2D, d_texture->value());
}

void FrameBuffer::unbindTexture() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

void FrameBuffer::setScreenSize(int width, int height)
{
    d_width = width;
    d_height = height;

    glBindTexture(GL_TEXTURE_2D, d_texture->value());
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width * d_scale, height * d_scale, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glBindTexture(GL_TEXTURE_2D, 0);

    glBindRenderbuffer(GL_RENDERBUFFER, d_depthBuffer->value());
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width * d_scale, height * d_scale);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

}