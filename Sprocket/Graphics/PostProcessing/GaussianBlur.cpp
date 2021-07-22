#include "GaussianBlur.h"

namespace spkt {

GaussianVert::GaussianVert(int width, int height)
    : Effect(width, height,
             "Resources/Shaders/GaussV.vert",
             "Resources/Shaders/Gauss.frag")
{

}

GaussianHoriz::GaussianHoriz(int width, int height)
    : Effect(width, height,
             "Resources/Shaders/GaussH.vert",
             "Resources/Shaders/Gauss.frag")
{

}
    
}