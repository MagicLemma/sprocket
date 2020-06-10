#include "Draggable.h"
#include "Widget.h"
#include "Quad.h"
#include "MouseEvent.h"
#include "MouseCodes.h"

namespace Sprocket {

Draggable::Draggable()
    : d_moving(false)
{
}

void Draggable::OnUpdate(Widget* widget)
{
    d_mouse.OnUpdate();
    if (d_moving) {
        auto currentPosition = widget->Position();
        widget->Position(currentPosition + d_mouse.GetMouseOffset());
    }
}

void Draggable::OnEvent(Widget* widget, Event& event)
{
    d_mouse.OnEvent(event);

    Quad widgetQuad{{0.0, 0.0}, widget->Width(), widget->Height()};
    if (ContainsPoint(widgetQuad, widget->ToLocalCoords(d_mouse.GetMousePos()))) {
        if (auto e = event.As<MouseButtonPressedEvent>()) {
            if (!e->IsConsumed() && e->Button() == Mouse::LEFT) {
                d_moving = true;
                e->Consume();
            }
        }
    }

    if (auto e = event.As<MouseButtonReleasedEvent>()) {
        d_moving = false;
    }
}
    
}