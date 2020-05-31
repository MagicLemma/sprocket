#include "VerticalConstraint.h"
#include "Log.h"
#include "Widget.h"

namespace Sprocket {

VerticalConstraint::VerticalConstraint(Type type, float offset, Window* window)
    : d_type(type)
    , d_offset(offset)
    , d_window(window)
{
}

void VerticalConstraint::OnUpdate(Widget* widget)
{
    auto current_position = widget->Position();

    switch (d_type) {
        case Type::TOP: {
            widget->Position({
                current_position.x,
                d_offset
            });
        } break;
        case Type::BOTTOM: {
            widget->Position({
                current_position.x,
                (float)d_window->Height() - widget->Height() - d_offset
            });
        } break;
        case Type::CENTRE: {
            widget->Position({
                current_position.x,
                ((float)d_window->Height() - widget->Height())/2.0f
            });
        } break;
        default: {
            SPKT_LOG_ERROR("VerticalConstraint encountered an unknown type!");
        }  
    }
}

}