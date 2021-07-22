#include "Effect.h"

#include <glad/glad.h>

#include <string>

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
    d_shader.LoadFloat("targetWidth", (float)Width());
    d_shader.LoadFloat("targetHeight", (float)Height());
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

void Effect::SetScreenSize(int width, int height)
{
    d_frameBuffer.SetScreenSize(width, height);
}
    
}