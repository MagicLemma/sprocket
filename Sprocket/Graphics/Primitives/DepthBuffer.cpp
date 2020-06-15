#include "DepthBuffer.h"

#include <glad/glad.h>

namespace Sprocket {

DepthBuffer::DepthBuffer(Window* window, int width, int height)
    : d_window(window)
    , d_fbo(std::make_shared<FBO>())
    , d_texture(std::make_shared<TEX>())
    , d_depth(std::make_shared<TEX>())
    , d_width(width)
    , d_height(height)
{
    glBindFramebuffer(GL_FRAMEBUFFER, d_fbo->Value());
    //glDrawBuffer(GL_NONE);
    //glReadBuffer(GL_NONE);

    // TODO: Remove Colour attachment
    glBindTexture(GL_TEXTURE_2D, d_texture->Value());
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, d_texture->Value(), 0);


    glBindTexture(GL_TEXTURE_2D, d_depth->Value());
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, width, height, 0,
                 GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColour[] = {1.0f, 1.0f, 1.0f, 1.0f};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColour);

    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, d_depth->Value(), 0);
    
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