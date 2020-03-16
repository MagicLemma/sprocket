#pragma once
#include "Quad.h"
#include "UiComponent.h"
#include "Window.h"
#include "Event.h"

#include <functional>

namespace Sprocket {

using SliderCallback = std::function<void(float)>;

struct SliderAttributes
{
    float barHeight = 0.2f;
    float barWidth = 0.9f;
        // Dimensions of the bar as percentages of the whole quad.

    float pickerHeight = 0.8f;
    float pickerWidth = 0.05f;
        // Dimensions of the picker as percentages of the whole quad.

    Maths::vec3 backgroundColour = {0.0f, 0.0f, 0.0f};
    Maths::vec3 barColour = {1.0f, 0.0f, 0.0f};
    Maths::vec3 pickerColour = {1.0f, 1.0f, 1.0f};
};

class Slider : public UiComponent
{
    SliderAttributes d_attributes;

    Quad d_background;
    Quad d_bar;
    Quad d_picker;

    float d_left;
    float d_right;
        // Values in pixels of how far the slider can move.
    
    bool d_updating;

    SliderCallback d_callback = [](float){};

public:
    Slider(const Maths::vec2& topLeft,
           float width,
           float height,
           float initialValue = 0.0f,
           const SliderAttributes& attrs = {});

    Quad background() const { return d_background; }
    Quad bar() const { return d_bar; }
    Quad picker() const { return d_picker; }

    const SliderAttributes& attributes() const { return d_attributes; }
    SliderAttributes& attributes() { return d_attributes; }

    void update(Window* window) override;
    void handleEvent(Window* window, const Event& event) override;

    float value() const;

    void setCallback(SliderCallback cb) { d_callback = cb; }
};

}