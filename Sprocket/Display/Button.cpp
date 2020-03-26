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
    , d_background({
        {0.0, 0.0},
        width,
        height
    })
    , d_backgroundVisuals({
        Texture::empty(),
        attrs.backgroundColour
    })
    , d_button({
        makeButtonOffset(width, height, attrs),
        width * attrs.buttonWidth,
        height * attrs.buttonHeight
    })
    , d_buttonVisuals({
        Texture::empty(),
        attrs.buttonColour
    })
    , d_attributes(attrs)
    , d_hovered(false)
    , d_clicked(false)
{
}

void Button::updateImpl(Window* window)
{
    if (d_clicked) {
        d_backgroundVisuals.colour = d_attributes.backgroundColourClicked;
        d_buttonVisuals.colour = d_attributes.buttonColourClicked;
    }
    else if (d_hovered) {
        d_backgroundVisuals.colour = d_attributes.backgroundColourHovered;
        d_buttonVisuals.colour = d_attributes.buttonColourHovered;
    }
    else {
        d_backgroundVisuals.colour = d_attributes.backgroundColour;
        d_buttonVisuals.colour = d_attributes.buttonColour;
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
    return containsPoint(background(), toLocalCoords(window->getMousePos()));
}

void Button::drawImpl(DisplayRenderer* renderer) const
{
    renderer->draw(toScreenCoords(d_background), d_backgroundVisuals);
    renderer->draw(toScreenCoords(d_button), d_buttonVisuals);
}

}