#include "GaussianBlur.h"

namespace Sprocket {

void GaussianVert::SetUniforms() const
{
    d_shader.LoadUniform("targetHeight", (float)Height());
}

GaussianVert::GaussianVert(int width, int height)
    : Effect(width, height,
             "Resources/Shaders/GaussV.vert",
             "Resources/Shaders/Gauss.frag")
{

}

void GaussianHoriz::SetUniforms() const
{
    d_shader.LoadUniform("targetWidth", (float)Width());
}

GaussianHoriz::GaussianHoriz(int width, int height)
    : Effect(width, height,
             "Resources/Shaders/GaussH.vert",
             "Resources/Shaders/Gauss.frag")
{

}
    
}