#include "Slider.h"
#include "MouseEvent.h"
#include "Maths.h"
#include "Log.h"

namespace Sprocket {
namespace {

Maths::vec2 makeBarOffset(float width, float height, const SliderAttributes& attrs)
{
    return {
        width * (1 - attrs.barWidth) / 2,
        height * (1 - attrs.barHeight) / 2
    };
}

Maths::vec2 makePickerOffset(float width, float height, const SliderAttributes& attrs)
{
    return {
        width * (1 - attrs.pickerWidth) / 2,
        height * (1 - attrs.pickerHeight) / 2
    };
}

}
    
Slider::Slider(float width,
               float height,
               float initialValue,
               const SliderAttributes& attrs)
    : Widget(width, height)
    , d_attributes(attrs)
    , d_updating(false)
{
    d_quads.push_back({
        {{0.0, 0.0}, width, height},
        attrs.backgroundColour
    });

    d_quads.push_back({
        {
            makeBarOffset(width, height, attrs), 
            attrs.barWidth * width,
            attrs.barHeight * height
        },
        attrs.barColour
    });

    d_quads.push_back({
        {
            makePickerOffset(width, height, attrs),
            attrs.pickerWidth * width,
            attrs.pickerHeight * height,
        },
        attrs.pickerColour
    });

    Maths::clamp(initialValue, 0.0f, 1.0f);
    picker().body.position.x = left() + initialValue * (right() - left());    
}

void Slider::updateImpl(Window* window)
{
    if (d_updating) {
        if (window->getMouseOffset().x != 0) {
            float newHorizPos = toLocalCoords(window->getMousePos()).x - picker().body.width/2;
            auto& offset = picker().body.position;
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
                float newHorizPos = toLocalCoords(window->getMousePos()).x - picker().body.width/2;
                auto& offset = picker().body.position;
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

float Slider::value() const
{
    return (picker().body.position.x - left()) / (right() - left());
}

float Slider::left() const
{
    return bar().body.position.x;
}

float Slider::right() const
{
    return bar().body.position.x + bar().body.width - picker().body.width;
}

}