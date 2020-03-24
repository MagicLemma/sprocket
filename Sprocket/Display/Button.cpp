#include "Button.h"
#include "MouseEvent.h"
#include "Log.h"

namespace Sprocket {
namespace {

Maths::vec2 makeButtonOffset(float width, float height, const ButtonAttributes& attrs)
{
    return {
        width * (1 - attrs.buttonWidth) / 2,
        height * (1 - attrs.buttonHeight) / 2
    };
}

}
    
Button::Button(float width,
               float height,
               const ButtonAttributes& attrs)
    : Widget(width, height)
    , d_attributes(attrs)
    , d_hovered(false)
    , d_clicked(false)
{
    d_quads.push_back({
        {{0.0, 0.0}, width, height},
        attrs.backgroundColour
    });

    d_quads.push_back({
        makeButtonOffset(width, height, attrs),
        width * attrs.buttonWidth,
        height * attrs.buttonHeight,
        attrs.buttonColour
    });
}

void Button::updateImpl(Window* window)
{
    if (d_clicked) {
        background().skin = d_attributes.backgroundColourClicked;
        button().skin = d_attributes.buttonColourClicked;
    }
    else if (d_hovered) {
        background().skin = d_attributes.backgroundColourHovered;
        button().skin = d_attributes.buttonColourHovered;
    }
    else {
        background().skin = d_attributes.backgroundColour;
        button().skin = d_attributes.buttonColour;
    }
}

bool Button::handleEventImpl(Window* window, const Event& event)
{
    if (auto e = event.as<MouseMovedEvent>()) {
        bool mouseOnButton = containsPoint(button(), toLocalCoords(window->getMousePos()));
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
            if (containsPoint(button(), toLocalCoords(window->getMousePos()))) {
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
    return containsPoint(background(), toLocalCoords(window->getMousePos()));
}

}