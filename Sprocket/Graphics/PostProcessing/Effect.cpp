#include "Effect.h"
#include "Log.h"

#include <glad/glad.h>

namespace Sprocket {
namespace {

Vertex2DBuffer getQuad()
{
    return Vertex2DBuffer{
        {
            Maths::vec2{-1.0f, -1.0f},
            Maths::vec2{0.0f, 0.0f},
            Maths::vec3{1.0f, 1.0f, 1.0f}
        }, {
            Maths::vec2{1.0f, -1.0f},
            Maths::vec2{1.0f, 0.0f},
            Maths::vec3{1.0f, 1.0f, 1.0f}
        }, {
            Maths::vec2{1.0f, 1.0f},
            Maths::vec2{1.0f, 1.0f},
            Maths::vec3{1.0f, 1.0f, 1.0f}
        }, {
            Maths::vec2{1.0f, 1.0f},
            Maths::vec2{1.0f, 1.0f},
            Maths::vec3{1.0f, 1.0f, 1.0f}
        }, {
            Maths::vec2{-1.0f, 1.0f},
            Maths::vec2{0.0f, 1.0f},
            Maths::vec3{1.0f, 1.0f, 1.0f}
        }, {
            Maths::vec2{-1.0f, -1.0f},
            Maths::vec2{0.0f, 0.0f},
            Maths::vec3{1.0f, 1.0f, 1.0f}
        }
    };
}

}

Effect::Effect(ResourceManager* resourceManager,
               int width, int height, float scale,
               const std::string& vertShader,
               const std::string& fragShader)
    : d_frameBuffer(resourceManager->loadFrameBuffer(width, height, scale))
    , d_shader(vertShader, fragShader)
    , d_quad(resourceManager->loadModel2D(getQuad()))
{
}

void Effect::bindForWrite() const
{
    d_frameBuffer.bind();
}

void Effect::unbindForWrite() const
{
    d_frameBuffer.unbind();
}

void Effect::bindForRead() const
{
    d_shader.bind();
    d_quad.bind();
    d_frameBuffer.bindTexture();
    setUniforms();
}

void Effect::unbindForRead() const
{
    d_frameBuffer.unbindTexture();
    d_quad.unbind();
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
    d_frameBuffer.setScreenSize(width, height);
}
    
}