#include "Button.h"
#include "MouseEvent.h"
#include "Log.h"

namespace Sprocket {
namespace {

Maths::vec2 makeButtonOffset(float width, float height, float buttonScale)
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
    , d_button({
        makeButtonOffset(width, height, buttonScale),
        width * buttonScale,
        height * buttonScale,
        Texture::empty()
    })
    , d_buttonHovered({
        makeButtonOffset(width, height, buttonHoveredScale),
        width * buttonHoveredScale,
        height * buttonHoveredScale,
        Texture::empty()
    })
    , d_buttonClicked({
        makeButtonOffset(width, height, buttonClickedScale),
        width * buttonClickedScale,
        height * buttonClickedScale,
        Texture::empty()
    })
    , d_hovered(false)
    , d_clicked(false)
{
    d_activeButton = &d_button;
}

void Button::updateImpl(Window* window)
{
    if (d_clicked) {
        d_activeButton = &d_buttonClicked;
    }
    else if (d_hovered) {
        d_activeButton = &d_buttonHovered;
    }
    else {
        d_activeButton = &d_button;
    }
}

bool Button::handleEventImpl(Window* window, const Event& event)
{
    if (auto e = event.as<MouseMovedEvent>()) {
        bool mouseOnButton = containsPoint(d_button, toLocalCoords(window->getMousePos()));
        if (!d_hovered && mouseOnButton) {
            d_hovered = true;
            d_hoverCallback();
            return false;
        }
        else if (d_hovered && !mouseOnButton) {
            d_hovered = false;
            d_unhoverCallback();
            return false;
        }
    }
    else if (auto e = event.as<MouseButtonPressedEvent>()) {
        if (e->button() == Mouse::LEFT) {
            if (containsPoint(d_button, toLocalCoords(window->getMousePos()))) {
                d_clicked = true;
                d_clickCallback();
                return true;
            }
        }   
    } else if (auto e = event.as<MouseButtonReleasedEvent>()) {
        if (d_clicked) {
            d_clicked = false;
            d_unclickCallback();
            return false;
        }
    }
    return containsPoint(d_base, toLocalCoords(window->getMousePos()));
}

void Button::drawImpl(DisplayRenderer* renderer) const
{
    renderer->draw(toScreenCoords(*d_activeButton));
}

}