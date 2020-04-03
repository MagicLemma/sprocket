#include "FrameBuffer.h"

#include <glad/glad.h>

namespace Sprocket {

FrameBuffer::FrameBuffer(
    unsigned int fboId,
    unsigned int texture,
    unsigned int depth,
    int width,
    int height,
    float scale
)
    : d_fboId(fboId)
    , d_textureId(texture)
    , d_depthId(depth)
    , d_width(width)
    , d_height(height)
    , d_scale(scale)
{
}

void FrameBuffer::bind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, d_fboId);
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
    glBindTexture(GL_TEXTURE_2D, d_textureId);
}

void FrameBuffer::unbindTexture() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

void FrameBuffer::setScreenSize(int width, int height)
{
    d_width = width;
    d_height = height;

    glBindTexture(GL_TEXTURE_2D, d_textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width * d_scale, height * d_scale, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glBindTexture(GL_TEXTURE_2D, 0);

    glBindRenderbuffer(GL_RENDERBUFFER, d_depthId);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width * d_scale, height * d_scale);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

}