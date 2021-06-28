#include "Effect.h"
#include "Log.h"

#include <glad/glad.h>

namespace spkt {

Effect::Effect(int width, int height,
               const std::string& vertShader,
               const std::string& fragShader)
    : d_frameBuffer(width, height)
    , d_shader(vertShader, fragShader)
{
}

void Effect::BindForWrite()
{
    d_frameBuffer.Bind();
}

void Effect::UnbindForWrite()
{
    d_frameBuffer.Unbind();
}

void Effect::BindForRead() const
{
    d_shader.Bind();
    d_frameBuffer.BindTexture();
    SetUniforms();
}

void Effect::UnbindForRead() const
{
    d_frameBuffer.UnbindTexture();
    d_shader.Unbind();
}

void Effect::Draw(Effect* target)
{
    if (target) { target->BindForWrite(); }
    BindForRead();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
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