#pragma once
#include "Quad.h"
#include "Widget.h"
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

class Slider : public Widget
{
    Quad        d_background;
    QuadVisuals d_backgroundVisuals;

    Quad        d_bar;
    QuadVisuals d_barVisuals;

    Quad        d_picker;
    QuadVisuals d_pickerVisuals;
    
    bool d_updating;

    SliderCallback d_callback = [](float){};

    float left() const;
    float right() const;
        // Maximum left and right positions of the picker in
        // Local Coordinates.

    void updateImpl(Window* window) override;
    bool handleEventImpl(Window* window, const Event& event) override;
    void drawImpl(DisplayRenderer* renderer) const override;
    
public:
    Slider(float width,
           float height,
           float initialValue,
           const SliderAttributes& attrs = {});

    Quad& background() { return d_background; }
    Quad& bar() { return d_bar; }
    Quad& picker() { return d_picker; }
    const Quad& background() const { return d_background; }
    const Quad& bar() const { return d_bar; }
    const Quad& picker() const { return d_picker; }

    QuadVisuals& backgroundVisuals() { return d_backgroundVisuals; }
    QuadVisuals& barVisuals() { return d_barVisuals; }
    QuadVisuals& pickerVisuals() { return d_pickerVisuals; }
    const QuadVisuals& backgroundVisuals() const { return d_backgroundVisuals; }
    const QuadVisuals& barVisuals() const { return d_barVisuals; }
    const QuadVisuals& pickerVisuals() const { return d_pickerVisuals; }

    float value() const;

    void setCallback(SliderCallback cb) { d_callback = cb; }
};

}