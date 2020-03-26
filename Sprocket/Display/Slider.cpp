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
    , d_background({
        {0.0, 0.0},
        width,
        height
    })
    , d_backgroundVisuals({
        Texture::empty(),
        attrs.backgroundColour
    })
    , d_bar({
        makeBarOffset(width, height, attrs), 
        attrs.barWidth * width,
        attrs.barHeight * height
    })
    , d_barVisuals({
        Texture::empty(),
        attrs.barColour
    })
    , d_picker({
        makePickerOffset(width, height, attrs),
        attrs.pickerWidth * width,
        attrs.pickerHeight * height
    })
    , d_pickerVisuals({
        Texture::empty(),
        attrs.pickerColour
    })
    , d_updating(false)
{
    Maths::clamp(initialValue, 0.0f, 1.0f);
    d_picker.position.x = left() + initialValue * (right() - left());    
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
    renderer->draw(toScreenCoords(d_background), d_backgroundVisuals);
    renderer->draw(toScreenCoords(d_bar), d_barVisuals);
    renderer->draw(toScreenCoords(d_picker), d_pickerVisuals);
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

}