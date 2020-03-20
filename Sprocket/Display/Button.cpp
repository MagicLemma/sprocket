#include "Button.h"
#include "MouseEvent.h"
#include "Log.h"

namespace Sprocket {
namespace {

Quad makeButtonQuad(const Quad& quad, const ButtonAttributes& attributes)
{
    float x = quad.left() + quad.width() * (1 - attributes.buttonWidth) / 2;
    float y = quad.top() + quad.height() * (1 - attributes.buttonHeight) / 2;
    float width = attributes.buttonWidth * quad.width();
    float height = attributes.buttonHeight * quad.height();
    return {{x, y}, width, height};
}

}
    
Button::Button(const Maths::vec2& topLeft,
               float width,
               float height,
               const ButtonAttributes& attributes)
    : d_attributes(attributes)
    , d_background({topLeft, width, height})
    , d_button(makeButtonQuad(d_background, attributes))
    , d_hovered(false)
    , d_clicked(false)
{
    d_background.colour(attributes.backgroundColour);
    d_button.colour(attributes.buttonColour);
}

void Button::update(Window* window)
{
    if (d_clicked) {
        d_background.colour(d_attributes.backgroundColourClicked);
        d_button.colour(d_attributes.buttonColourClicked);
    }
    else if (d_hovered) {
        d_background.colour(d_attributes.backgroundColourHovered);
        d_button.colour(d_attributes.buttonColourHovered);
    }
    else {
        d_background.colour(d_attributes.backgroundColour);
        d_button.colour(d_attributes.buttonColour);
    }
}

void Button::handleEvent(Window* window, const Event& event)
{
    if (auto e = event.as<MouseMovedEvent>()) {
        bool mouseOnButton = d_button.containsPoint(window->getMousePos());
        if (!d_hovered && mouseOnButton) {
            d_hovered = true;
            d_hoverCallback();
        }
        else if (d_hovered && !mouseOnButton) {
            d_hovered = false;
            d_unhoverCallback();
        }
    }
    else if (auto e = event.as<MouseButtonPressedEvent>()) {
        if (e->button() == Mouse::LEFT) {
            if (d_button.containsPoint(window->getMousePos())) {
                d_clicked = true;
                d_clickCallback();
            }
        }   
    } else if (auto e = event.as<MouseButtonReleasedEvent>()) {
        if (d_clicked) {
            d_clicked = false;
            d_unclickCallback();
        }
    }
}

}