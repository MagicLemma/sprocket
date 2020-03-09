#include "Effect.h"
#include "Loader.h"
#include "Log.h"

#include <glad/glad.h>

namespace Sprocket {
namespace {

std::shared_ptr<Model2D> getQuad()
{
    Vertex2DBuffer v{
        {Maths::vec2{-1.0f, -1.0f}, Maths::vec2{0.0f, 0.0f}},
        {Maths::vec2{1.0f, -1.0f}, Maths::vec2{1.0f, 0.0f}},
        {Maths::vec2{1.0f, 1.0f}, Maths::vec2{1.0f, 1.0f}},
        {Maths::vec2{1.0f, 1.0f}, Maths::vec2{1.0f, 1.0f}},
        {Maths::vec2{-1.0f, 1.0f}, Maths::vec2{0.0f, 1.0f}},
        {Maths::vec2{-1.0f, -1.0f}, Maths::vec2{0.0f, 0.0f}}
    };
    return Loader::loadModel2D(v);
}

}

Effect::Effect(int width, int height, float scale,
               const std::string& vertShader,
               const std::string& fragShader)
    : d_fboId(0)
    , d_texture(0)
    , d_depth(0)
    , d_width(width)
    , d_height(height)
    , d_scale(scale)
    , d_shader(vertShader, fragShader)
    , d_quad(getQuad())
{
    d_fboId = Loader::createFBO();

    // Add texture to render to.
    d_texture = Loader::createTEX(Loader::TextureType::FLAT);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width * scale, height * scale, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, d_texture, 0);

    // Add depth buffer.
    d_depth = Loader::createRBO();
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width * scale, height * scale);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, d_depth);

    // Validate the framebuffer.
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        SPKT_LOG_ERROR("Created FBO is not complete!");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Effect::bindForWrite() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, d_fboId);
    glViewport(0, 0, d_width * d_scale, d_height * d_scale);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
}

void Effect::unbindForWrite() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, d_width, d_height);
}

void Effect::bindForRead() const
{
    d_shader.bind();
    d_quad->bind();
    setUniforms();
    glBindTexture(GL_TEXTURE_2D, d_texture);
}

void Effect::unbindForRead() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
    d_quad->unbind();
    d_shader.unbind();
}

void Effect::draw(const std::shared_ptr<Effect> target)
{
    if (target) { target->bindForWrite(); }
    bindForRead();
    glDrawArrays(GL_TRIANGLES, 0, 6);
    unbindForRead();
    if (target) { target->unbindForWrite(); }
}

void Effect::setUniforms() const
{
    // Override this function to set up uniforms.
}

void Effect::setScreenSize(int width, int height)
{
    d_width = width;
    d_height = height;

    glBindTexture(GL_TEXTURE_2D, d_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width * d_scale, height * d_scale, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glBindTexture(GL_TEXTURE_2D, 0);

    glBindRenderbuffer(GL_RENDERBUFFER, d_depth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width * d_scale, height * d_scale);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}
    
}