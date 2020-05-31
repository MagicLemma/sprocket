#include "HorizontalConstraint.h"
#include "Log.h"
#include "Widget.h"

namespace Sprocket {

HorizontalConstraint::HorizontalConstraint(Type type, float offset, Window* window)
    : d_type(type)
    , d_offset(offset)
    , d_window(window)
{
}

void HorizontalConstraint::OnUpdate(Widget* widget)
{
    auto current_position = widget->Position();

    switch (d_type) {
        case Type::LEFT: {
            widget->Position({
                d_offset,
                current_position.y
            });
        } break;
        case Type::RIGHT: {
            widget->Position({
                (float)d_window->Width() - widget->Width() - d_offset,
                current_position.y
            });
        } break;
        case Type::CENTRE: {
            widget->Position({
                ((float)d_window->Width() - widget->Width())/2.0f,
                current_position.y
            });
        } break;
        default: {
            SPKT_LOG_ERROR("HorizontalConstraint encountered an unknown type!");
        }  
    }
}

}