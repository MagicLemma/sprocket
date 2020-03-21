#include "VerticalConstraint.h"
#include "Log.h"

namespace Sprocket {

VerticalConstraint::VerticalConstraint(Type type, float offset)
    : d_type(type)
    , d_offset(offset)
{
}

void VerticalConstraint::apply(Window* window, Quad& quad)
{
    switch (d_type) {
        case Type::TOP: {
            quad.yCoord(d_offset);
        } break;
        case Type::BOTTOM: {
            quad.yCoord((float)window->height() - quad.height() - d_offset);
        } break;
        case Type::CENTRE: {
            quad.yCoord(((float)window->height() - quad.height())/2.0f);
        } break;
        default: {
            SPKT_LOG_ERROR("VerticalConstraint encountered an unknown type!");
        }  
    }
}

void VerticalConstraint::apply(Window* window, Image& image)
{
    apply(window, image.quad());
}

}