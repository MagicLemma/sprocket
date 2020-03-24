#include "Draggable.h"
#include "Widget.h"
#include "Quad.h"
#include "MouseEvent.h"

namespace Sprocket {

Draggable::Draggable()
    : d_moving(false)
{
}

void Draggable::update(Widget* widget, Window* window)
{
    if (d_moving) {
        auto currentPosition = widget->position();
        widget->position(currentPosition + window->getMouseOffset());
    }
}

bool Draggable::handleEvent(Widget* widget,
                          Window* window,
                          const Event& event)
{
    Quad widgetQuad{{0.0, 0.0}, widget->width(), widget->height()};
    if (containsPoint(widgetQuad, widget->toLocalCoords(window->getMousePos()))) {
        if (auto e = event.as<MouseButtonPressedEvent>()) {
            if (e->button() == Mouse::LEFT) {
                d_moving = true;
                return true;
            }
        }
    }
    
    if (auto e = event.as<MouseButtonReleasedEvent>()) {
        d_moving = false;
    }

    return false;
}
    
}