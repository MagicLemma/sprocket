#include "AspectConstraint.h"
#include "Log.h"

namespace Sprocket {

AspectConstraint::AspectConstraint(Base base, float aspectRatio)
    : d_base(base)
    , d_aspectRatio(aspectRatio)
{
}

void AspectConstraint::apply(Window* window, Quad& quad)
{
    switch (d_base) {
        case Base::WIDTH: {
            quad.height(quad.width() / d_aspectRatio);
        } break;
        case Base::HEIGHT: {
            quad.width(quad.height() * d_aspectRatio);
        } break;
        
        default: {
            SPKT_LOG_ERROR("AspectConstraint encountered an unknown base!");
        }  
    }
}

void AspectConstraint::apply(Window* window, Image& image)
{
    apply(window, image.quad());
}
    
}