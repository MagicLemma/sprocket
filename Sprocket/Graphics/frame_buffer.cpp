#include "frame_buffer.h"

#include <Sprocket/Graphics/Texture.h>
#include <Sprocket/Core/log.h>

#include <glad/glad.h>

#include <memory>

namespace spkt {

frame_buffer::frame_buffer(int width, int height)
    : d_colour(std::make_unique<texture>(width, height, texture_channels::RGBA))
    , d_depth(std::make_unique<texture>(width, height, texture_channels::DEPTH))
    , d_width(width)
    , d_height(height)
    , d_fbo(0)
{
    resize(width, height);
}

frame_buffer::~frame_buffer()
{
    glDeleteFramebuffers(1, &d_fbo);
}

void frame_buffer::bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, d_fbo);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    d_viewport.set(0, 0, d_width, d_height);
}

void frame_buffer::unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    d_viewport.restore();
}

void frame_buffer::resize(int width, int height)
{
    if (d_fbo) { // When called from constructor, nothing to delete, and no need to resize
        if (width == d_width && height == d_height) {
            return; // Nothing to do;
        }

        glDeleteFramebuffers(1, &d_fbo);
        d_colour->resize(width, height);
        d_depth->resize(width, height);
    }

    glCreateFramebuffers(1, &d_fbo);
    glNamedFramebufferTexture(d_fbo, GL_COLOR_ATTACHMENT0, d_colour->id(), 0);
    glNamedFramebufferTexture(d_fbo, GL_DEPTH_ATTACHMENT, d_depth->id(), 0);

    // Validate the framebuffer.
    if (glCheckNamedFramebufferStatus(d_fbo, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        log::error("Created FBO is not complete!");
    }

    d_width = width;
    d_height = height;
}

}