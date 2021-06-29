#include "GaussianBlur.h"

namespace spkt {

void GaussianVert::SetUniforms() const
{
    d_shader.LoadFloat("targetHeight", (float)Height());
}

GaussianVert::GaussianVert(int width, int height)
    : Effect(width, height,
             "Resources/Shaders/GaussV.vert",
             "Resources/Shaders/Gauss.frag")
{

}

void GaussianHoriz::SetUniforms() const
{
    d_shader.LoadFloat("targetWidth", (float)Width());
}

GaussianHoriz::GaussianHoriz(int width, int height)
    : Effect(width, height,
             "Resources/Shaders/GaussH.vert",
             "Resources/Shaders/Gauss.frag")
{

}
    
}