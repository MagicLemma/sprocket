#include "Slider.h"
#include "MouseEvent.h"
#include "Maths.h"
#include "Log.h"

namespace Sprocket {
namespace {

Quad makeBarQuad(const Quad& quad, const SliderAttributes& attributes)
{
    float x = quad.left() + quad.width() * (1 - attributes.barWidth) / 2;
    float y = quad.top() + quad.height() * (1 - attributes.barHeight) / 2;
    float width = attributes.barWidth * quad.width();
    float height = attributes.barHeight * quad.height();
    return {{x, y}, width, height};
}

Quad makePickerQuad(const Quad& quad, const SliderAttributes& attributes)
{
    float x = quad.left() + quad.width() * (1 - attributes.pickerWidth) / 2;
    float y = quad.top() + quad.height() * (1 - attributes.pickerHeight) / 2;
    float width = attributes.pickerWidth * quad.width();
    float height = attributes.pickerHeight * quad.height();
    return {{x, y}, width, height};
}

}
    
Slider::Slider(const Maths::vec2& topLeft,
               float width,
               float height,
               float initialValue,
               const SliderAttributes& attributes)
    : d_attributes(attributes)
    , d_background({topLeft, width, height})
    , d_bar(makeBarQuad(d_background, attributes))
    , d_picker(makePickerQuad(d_background, attributes))
    , d_left(d_bar.left())
    , d_right(d_bar.right() - d_picker.width())
    , d_updating(false)
{
    Maths::clamp(initialValue, 0.0f, 1.0f);
    d_picker.xCoord(d_left + initialValue * (d_right - d_left));
    
    d_background.colour(attributes.backgroundColour);
    d_bar.colour(attributes.barColour);
    d_picker.colour(attributes.pickerColour);
}

void Slider::update(Window* window)
{
    if (d_updating) {
        if (window->getMouseOffset().x != 0) {
            float newHorizPos = window->getMousePos().x - d_picker.width()/2;
            Maths::clamp(newHorizPos, d_left, d_right);
            d_picker.xCoord(newHorizPos);
            d_callback(value());
        }
    }
}

void Slider::handleEvent(Window* window, const Event& event)
{
    if (d_picker.containsPoint(window->getMousePos())) {
        if (auto e = event.as<MouseButtonPressedEvent>()) {
            if (e->button() == Mouse::LEFT) {
                d_updating = true;
            }
        }       
    } else if (d_bar.containsPoint(window->getMousePos())) {
        if (auto e = event.as<MouseButtonPressedEvent>()) {
            if (e->button() == Mouse::LEFT) {
                d_updating = true;
                float newHorizPos = window->getMousePos().x - d_picker.width()/2;
                Maths::clamp(newHorizPos, d_left, d_right);
                d_picker.xCoord(newHorizPos);
                d_callback(value());
            }
        }
    }

    if (auto e = event.as<MouseButtonReleasedEvent>()) {
        if (e->button() == Mouse::LEFT) {
            d_updating = false;
        }
    }
}

float Slider::value() const
{
    return (d_picker.topLeft().x - d_left) / (d_right - d_left);
}

}