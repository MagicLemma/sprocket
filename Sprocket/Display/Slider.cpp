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

void Slider::updateImpl(DisplayRenderer* renderer)
{
    if (d_updating) {
        if (d_mouse.getMouseOffset().x != 0) {
            float newHorizPos = toLocalCoords(d_mouse.getMousePos()).x - d_picker.width/2;
            auto& offset = d_picker.position;
            Maths::clamp(newHorizPos, left(), right());
            offset.x = newHorizPos;
            d_callback(value());
        }
    }

    renderer->draw(toScreenCoords(d_bar));
    renderer->draw(toScreenCoords(d_picker));
}

void Slider::handleEventImpl(Event& event)
{
    if (containsPoint(picker(), toLocalCoords(d_mouse.getMousePos()))) {
        if (auto e = event.as<MouseButtonPressedEvent>()) {
            if (!e->isConsumed() && e->button() == Mouse::LEFT) {
                d_updating = true;
                e->consume();
            }
        }       
    } else if (containsPoint(bar(), toLocalCoords(d_mouse.getMousePos()))) {
        if (auto e = event.as<MouseButtonPressedEvent>()) {
            if (!e->isConsumed() && e->button() == Mouse::LEFT) {
                d_updating = true;
                float newHorizPos = toLocalCoords(d_mouse.getMousePos()).x - d_picker.width/2;
                auto& offset = d_picker.position;
                Maths::clamp(newHorizPos, left(), right());
                offset.x = newHorizPos;
                d_callback(value());
                e->consume();
            }
        }
    } else if (auto e = event.as<MouseButtonReleasedEvent>()) {
        if (!e->isConsumed() && e->button() == Mouse::LEFT) {
            d_updating = false;
        }
    }
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