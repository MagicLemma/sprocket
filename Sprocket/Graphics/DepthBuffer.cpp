#include "DepthBuffer.h"
#include "Log.h"

#include <glad/glad.h>

namespace spkt {

DepthBuffer::DepthBuffer(int width, int height)
    : d_depth(std::make_unique<Texture>(width, height, Texture::Channels::DEPTH))
    , d_width(width)
    , d_height(height)
{
    glGenFramebuffers(1, &d_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, d_fbo);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    glBindTexture(GL_TEXTURE_2D, d_depth->Id());
    float borderColour[] = {1.0f, 1.0f, 1.0f, 1.0f};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColour);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, d_depth->Id(), 0);
    
     // Validate the framebuffer.
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        log::error("Created FBO is not complete!");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

DepthBuffer::~DepthBuffer()
{
    glDeleteFramebuffers(1, &d_fbo);
}

void DepthBuffer::Bind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, d_fbo);
    d_viewport.Set(0, 0, d_width, d_height);
}

void DepthBuffer::Unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    d_viewport.Restore();
}

}