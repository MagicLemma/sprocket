#include "HorizontalConstraint.h"
#include "Log.h"

namespace Sprocket {

HorizontalConstraint::HorizontalConstraint(Type type, float offset)
    : d_type(type)
    , d_offset(offset)
{
}

void HorizontalConstraint::apply(Window* window, Quad& quad)
{
    switch (d_type) {
        case Type::LEFT: {
            quad.xCoord(d_offset);
        } break;
        case Type::RIGHT: {
            quad.xCoord((float)window->width() - quad.width() - d_offset);
        } break;
        case Type::CENTRE: {
            quad.xCoord(((float)window->width() - quad.width())/2.0f);
        } break;
        default: {
            SPKT_LOG_ERROR("HorizontalConstraint encountered an unknown type!");
        }  
    }
}

void HorizontalConstraint::apply(Window* window, Image& image)
{
    apply(window, image.quad());
}

}