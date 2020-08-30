#include "Effect.h"
#include "Log.h"

#include <glad/glad.h>

namespace Sprocket {
namespace {

Vertex2DBuffer GetQuad()
{
    return Vertex2DBuffer{
        {
            Maths::vec3{-1.0f, -1.0f, 0.0f},
            Maths::vec2{0.0f, 0.0f}
        }, {
            Maths::vec3{1.0f, -1.0f, 0.0f},
            Maths::vec2{1.0f, 0.0f}
        }, {
            Maths::vec3{1.0f, 1.0f, 0.0f},
            Maths::vec2{1.0f, 1.0f}
        }, {
            Maths::vec3{1.0f, 1.0f, 0.0f},
            Maths::vec2{1.0f, 1.0f}
        }, {
            Maths::vec3{-1.0f, 1.0f, 0.0f},
            Maths::vec2{0.0f, 1.0f}
        }, {
            Maths::vec3{-1.0f, -1.0f, 0.0f},
            Maths::vec2{0.0f, 0.0f}
        }
    };
}

}

Effect::Effect(int width, int height,
               const std::string& vertShader,
               const std::string& fragShader)
    : d_frameBuffer(width, height)
    , d_shader(vertShader, fragShader)
    , d_quad(GetQuad())
{
}

void Effect::BindForWrite() const
{
    d_frameBuffer.Bind();
}

void Effect::UnbindForWrite() const
{
    d_frameBuffer.Unbind();
}

void Effect::BindForRead() const
{
    d_shader.Bind();
    d_quad.Bind();
    d_frameBuffer.BindTexture();
    SetUniforms();
}

void Effect::UnbindForRead() const
{
    d_frameBuffer.UnbindTexture();
    d_quad.Unbind();
    d_shader.Unbind();
}

void Effect::Draw(const std::shared_ptr<Effect> target)
{
    if (target) { target->BindForWrite(); }
    BindForRead();
    glDrawArrays(GL_TRIANGLES, 0, 6);
    UnbindForRead();
    if (target) { target->UnbindForWrite(); }
}

void Effect::SetUniforms() const
{
    // Override this function to set up uniforms.
}

void Effect::SetScreenSize(int width, int height)
{
    d_frameBuffer.SetScreenSize(width, height);
}
    
}