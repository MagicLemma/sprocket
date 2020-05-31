#include "Button.h"
#include "MouseEvent.h"
#include "Log.h"

namespace Sprocket {
namespace {

Maths::vec2 MakeButtonOffset(float width, float height, float buttonScale)
{
    return {
        width * (1 - buttonScale) / 2,
        height * (1 - buttonScale) / 2
    };
}

}
    
Button::Button(float width,
               float height,
               float buttonScale,
               float buttonHoveredScale,
               float buttonClickedScale)
    : Widget(width, height)
    , d_buttonNormal({
        MakeButtonOffset(width, height, buttonScale),
        width * buttonScale,
        height * buttonScale
    })
    , d_buttonHovered({
        MakeButtonOffset(width, height, buttonHoveredScale),
        width * buttonHoveredScale,
        height * buttonHoveredScale
    })
    , d_buttonClicked({
        MakeButtonOffset(width, height, buttonClickedScale),
        width * buttonClickedScale,
        height * buttonClickedScale
    })
    , d_hovered(false)
    , d_clicked(false)
{
    d_actual = d_buttonNormal;
}

void Button::OnUpdateImpl(DisplayRenderer* renderer)
{
    float speed = 0.2f;

    if (d_clicked) {
        d_actual.position += speed * (d_buttonClicked.position - d_actual.position);
        d_actual.width += speed * (d_buttonClicked.width - d_actual.width);
        d_actual.height += speed * (d_buttonClicked.height - d_actual.height);

        d_actual.texture = d_buttonClicked.texture;
        d_actual.colour += speed * (d_buttonClicked.colour - d_actual.colour);
    }
    else if (d_hovered) {
        d_actual.position += speed * (d_buttonHovered.position - d_actual.position);
        d_actual.width += speed * (d_buttonHovered.width - d_actual.width);
        d_actual.height += speed * (d_buttonHovered.height - d_actual.height);

        d_actual.texture = d_buttonHovered.texture;
        d_actual.colour += speed * (d_buttonHovered.colour - d_actual.colour);
    }
    else {
        d_actual = d_buttonNormal;
    }

    renderer->Draw(ToScreenCoords(d_actual));
}

void Button::OnEventImpl(Event& event)
{
    if (auto e = event.As<MouseMovedEvent>()) {
        bool mouseOnButton = ContainsPoint(d_actual, ToLocalCoords(d_mouse.GetMousePos()));
        if (!d_hovered && mouseOnButton) {
            d_hovered = true;
            d_hoverCallback();
        }
        else if (d_hovered && !mouseOnButton) {
            d_hovered = false;
            d_unhoverCallback();
        }
    }
    else if (auto e = event.As<MouseButtonPressedEvent>()) {
        if (!e->IsConsumed() && e->Button() == Mouse::LEFT) {
            if (ContainsPoint(d_actual, ToLocalCoords(d_mouse.GetMousePos()))) {
                d_clicked = true;
                d_clickCallback();
                e->Consume();
            }
        }   
    } else if (auto e = event.As<MouseButtonReleasedEvent>()) {
        if (d_clicked) {
            d_clicked = false;
            d_unclickCallback();
        }
    }
    else if (ContainsPoint(d_base, ToLocalCoords(d_mouse.GetMousePos()))) {
        event.Consume();
    }
}

}