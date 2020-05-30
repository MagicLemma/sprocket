#include "GaussianBlur.h"

namespace Sprocket {

void GaussianVert::setUniforms() const
{
    d_shader.loadUniform("targetHeight", (float)height());
}

GaussianVert::GaussianVert(int width, int height)
    : Effect(width, height,
             "Resources/Shaders/GaussV.vert",
             "Resources/Shaders/Gauss.frag")
{

}

void GaussianHoriz::setUniforms() const
{
    d_shader.loadUniform("targetWidth", (float)width());
}

GaussianHoriz::GaussianHoriz(int width, int height)
    : Effect(width, height,
             "Resources/Shaders/GaussH.vert",
             "Resources/Shaders/Gauss.frag")
{

}
    
}