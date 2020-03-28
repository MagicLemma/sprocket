#include "InteractiveQuad.h"
#include "MouseEvent.h"

namespace Sprocket {

InteractiveQuad::InteractiveQuad(
    const Quad& normal,
    const Quad& hovered,
    const Quad& clicked
)
    : d_actual(normal)
    , d_normal(normal)
    , d_hovered(hovered)
    , d_clicked(clicked)
    , d_isHovered(false)
    , d_isClicked(false)
{
}

void InteractiveQuad::update(Window* window)
{
    float speed = 0.25f;

    if (d_isClicked) {
        d_actual.position += speed * (d_clicked.position - d_actual.position);
        d_actual.width += speed * (d_clicked.width - d_actual.width);
        d_actual.height += speed * (d_clicked.height - d_actual.height);

        d_actual.texture = d_clicked.texture;
        d_actual.colour += speed * (d_clicked.colour - d_actual.colour);
    }
    else if (d_isHovered) {
        d_actual.position += speed * (d_hovered.position - d_actual.position);
        d_actual.width += speed * (d_hovered.width - d_actual.width);
        d_actual.height += speed * (d_hovered.height - d_actual.height);

        d_actual.texture = d_hovered.texture;
        d_actual.colour += speed * (d_hovered.colour - d_actual.colour);
    }
    else {
        d_actual.position += speed * (d_normal.position - d_actual.position);
        d_actual.width += speed * (d_normal.width - d_actual.width);
        d_actual.height += speed * (d_normal.height - d_actual.height);

        d_actual.texture = d_normal.texture;
        d_actual.colour += speed * (d_normal.colour - d_actual.colour);
    }
}

bool InteractiveQuad::handleEvent(Window* window, const Event& event)
{
    if (auto e = event.as<MouseMovedEvent>()) {
        bool mouseOnButton = containsPoint(d_normal, window->getMousePos());
        if (d_isHovered) {
            mouseOnButton |= containsPoint(d_hovered, window->getMousePos());
        }
        if (d_isClicked) {
            mouseOnButton |= containsPoint(d_clicked, window->getMousePos());
        }
        
        if (!d_isHovered && mouseOnButton) {
            d_isHovered = true;
            return false;
        }
        else if (d_isHovered && !mouseOnButton) {
            d_isHovered = false;
            return false;
        }
    }
    else if (auto e = event.as<MouseButtonPressedEvent>()) {
        if (e->button() == Mouse::LEFT) {
            if (containsPoint(d_actual, window->getMousePos())) {
                d_isClicked = true;
                return true;
            }
        }   
    } else if (auto e = event.as<MouseButtonReleasedEvent>()) {
        if (d_isClicked) {
            d_isClicked = false;
            return false;
        }
    }
    return containsPoint(d_actual, window->getMousePos());
}

void InteractiveQuad::draw(DisplayRenderer* renderer)
{
    renderer->draw(d_actual);
}

}