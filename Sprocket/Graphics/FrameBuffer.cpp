#include "FrameBuffer.h"

#include <Sprocket/Graphics/Texture.h>
#include <Sprocket/Utility/Log.h>

#include <glad/glad.h>

#include <memory>

namespace spkt {

FrameBuffer::FrameBuffer(int width, int height)
    : d_colour(std::make_unique<Texture>(width, height, Texture::Channels::RGBA))
    , d_depth(std::make_unique<Texture>(width, height, Texture::Channels::DEPTH))
    , d_width(width)
    , d_height(height)
    , d_fbo(0)
{
    SetScreenSize(width, height);
}

FrameBuffer::~FrameBuffer()
{
    glDeleteFramebuffers(1, &d_fbo);
}

void FrameBuffer::Bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, d_fbo);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    d_viewport.Set(0, 0, d_width, d_height);
}

void FrameBuffer::Unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    d_viewport.Restore();
}

void FrameBuffer::BindTexture() const
{
    glBindTexture(GL_TEXTURE_2D, d_colour->Id());
}

void FrameBuffer::UnbindTexture() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

void FrameBuffer::SetScreenSize(int width, int height)
{
    if (d_fbo) { // When called from constructor, nothing to delete, and no need to resize
        if (width == d_width && height == d_height) {
            return; // Nothing to do;
        }

        glDeleteFramebuffers(1, &d_fbo);
        d_colour->Resize(width, height);
        d_depth->Resize(width, height);
    }

    glCreateFramebuffers(1, &d_fbo);
    glNamedFramebufferTexture(d_fbo, GL_COLOR_ATTACHMENT0, d_colour->Id(), 0);
    glNamedFramebufferTexture(d_fbo, GL_DEPTH_ATTACHMENT, d_depth->Id(), 0);

    // Validate the framebuffer.
    if (glCheckNamedFramebufferStatus(d_fbo, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        log::error("Created FBO is not complete!");
    }

    d_width = width;
    d_height = height;
}

}