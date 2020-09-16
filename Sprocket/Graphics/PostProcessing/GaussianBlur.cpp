#include "GaussianBlur.h"

namespace Sprocket {

void GaussianVert::SetUniforms() const
{
    d_shader.LoadUniformFloat("targetHeight", (float)Height());
}

GaussianVert::GaussianVert(int width, int height)
    : Effect(width, height,
             "Resources/Shaders/GaussV.vert",
             "Resources/Shaders/Gauss.frag")
{

}

void GaussianHoriz::SetUniforms() const
{
    d_shader.LoadUniformFloat("targetWidth", (float)Width());
}

GaussianHoriz::GaussianHoriz(int width, int height)
    : Effect(width, height,
             "Resources/Shaders/GaussH.vert",
             "Resources/Shaders/Gauss.frag")
{

}
    
}