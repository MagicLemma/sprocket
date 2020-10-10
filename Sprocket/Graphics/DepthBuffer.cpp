#include "DepthBuffer.h"

#include <glad/glad.h>

namespace Sprocket {

DepthBuffer::DepthBuffer(Window* window, int width, int height)
    : d_window(window)
    , d_fbo(std::make_shared<FBO>())
    , d_depth(std::make_shared<Texture>(width, height, Texture::Channels::DEPTH))
    , d_width(width)
    , d_height(height)
{
    glBindFramebuffer(GL_FRAMEBUFFER, d_fbo->Value());
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    glBindTexture(GL_TEXTURE_2D, d_depth->Id());
    float borderColour[] = {1.0f, 1.0f, 1.0f, 1.0f};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColour);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, d_depth->Id(), 0);
    
     // Validate the framebuffer.
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        SPKT_LOG_ERROR("Created FBO is not complete!");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void DepthBuffer::Bind() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, d_fbo->Value());
    glViewport(0, 0, d_width, d_height);
}

void DepthBuffer::Unbind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, d_window->Width(), d_window->Height());
}

}