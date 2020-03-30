#include "Slider.h"
#include "MouseEvent.h"
#include "Maths.h"
#include "Log.h"

namespace Sprocket {
    
Slider::Slider(float width, float height)
    : Widget(width, height)
    , d_bar({{0.0, 0.0}, 1.0f, 1.0f})
    , d_picker({{0.0, 0.0}, 1.0f, 1.0f})
    , d_updating(false)
{
}

void Slider::updateImpl(Window* window)
{
    if (d_updating) {
        if (window->getMouseOffset().x != 0) {
            float newHorizPos = toLocalCoords(window->getMousePos()).x - d_picker.width/2;
            auto& offset = d_picker.position;
            Maths::clamp(newHorizPos, left(), right());
            offset.x = newHorizPos;
            d_callback(value());
        }
    }
}

bool Slider::handleEventImpl(Window* window, const Event& event)
{
    if (containsPoint(picker(), toLocalCoords(window->getMousePos()))) {
        if (auto e = event.as<MouseButtonPressedEvent>()) {
            if (e->button() == Mouse::LEFT) {
                d_updating = true;
                return true;
            }
        }       
    } else if (containsPoint(bar(), toLocalCoords(window->getMousePos()))) {
        if (auto e = event.as<MouseButtonPressedEvent>()) {
            if (e->button() == Mouse::LEFT) {
                d_updating = true;
                float newHorizPos = toLocalCoords(window->getMousePos()).x - d_picker.width/2;
                auto& offset = d_picker.position;
                Maths::clamp(newHorizPos, left(), right());
                offset.x = newHorizPos;
                d_callback(value());
                return true;
            }
        }
    }

    if (auto e = event.as<MouseButtonReleasedEvent>()) {
        if (e->button() == Mouse::LEFT) {
            d_updating = false;
            return false;
        }
    }
    return false;
}

void Slider::drawImpl(DisplayRenderer* renderer) const
{
    renderer->draw(toScreenCoords(d_bar));
    renderer->draw(toScreenCoords(d_picker));
}

float Slider::value() const
{
    return (d_picker.position.x - left()) / (right() - left());
}

float Slider::left() const
{
    return d_bar.position.x;
}

float Slider::right() const
{
    return d_bar.position.x + d_bar.width - d_picker.width;
}

void Slider::setValue(float val)
{
    Maths::clamp(val, 0.0f, 1.0f);
    d_picker.position.x = left() + val * (right() - left());
    d_callback(val);
}

}