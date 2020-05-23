#include "HorizontalConstraint.h"
#include "Log.h"
#include "Widget.h"

namespace Sprocket {

HorizontalConstraint::HorizontalConstraint(Type type, float offset)
    : d_type(type)
    , d_offset(offset)
{
}

void HorizontalConstraint::update(Widget* widget, Window* window)
{
    auto current_position = widget->position();

    switch (d_type) {
        case Type::LEFT: {
            widget->position({
                d_offset,
                current_position.y
            });
        } break;
        case Type::RIGHT: {
            widget->position({
                (float)window->width() - widget->width() - d_offset,
                current_position.y
            });
        } break;
        case Type::CENTRE: {
            widget->position({
                ((float)window->width() - widget->width())/2.0f,
                current_position.y
            });
        } break;
        default: {
            SPKT_LOG_ERROR("HorizontalConstraint encountered an unknown type!");
        }  
    }
}

}