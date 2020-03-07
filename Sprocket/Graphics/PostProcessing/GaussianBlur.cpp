#include "GaussianBlur.h"

namespace Sprocket {

void GaussianVert::setUniforms() const
{
    d_shader.loadUniform("targetHeight", d_height);
}

GaussianVert::GaussianVert(int width, int height)
    : Effect(width, height, 0.125, "Resources/Shaders/GaussV.vert",
                               "Resources/Shaders/Gauss.frag")
{

}

void GaussianHoriz::setUniforms() const
{
    d_shader.loadUniform("targetWidth", d_width);
}

GaussianHoriz::GaussianHoriz(int width, int height)
    : Effect(width, height, 0.125, "Resources/Shaders/GaussH.vert",
                               "Resources/Shaders/Gauss.frag")
{

}
    
}