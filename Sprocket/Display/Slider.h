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
    SliderAttributes d_attributes;
    
    bool d_updating;

    SliderCallback d_callback = [](float){};

    float left() const;
    float right() const;
        // Maximum left and right positions of the picker in
        // Local Coordinates.

    void updateImpl(Window* window) override;
    bool handleEventImpl(Window* window, const Event& event) override;

public:
    Slider(float width,
           float height,
           float initialValue,
           const SliderAttributes& attrs = {});

    VisualQuad& background() { return d_quads[0]; }
    VisualQuad& bar() { return d_quads[1]; }
    VisualQuad& picker() { return d_quads[2]; }

    const VisualQuad& background() const { return d_quads[0]; }
    const VisualQuad& bar() const { return d_quads[1]; }
    const VisualQuad& picker() const { return d_quads[2]; }

    SliderAttributes& attributes() { return d_attributes; }
    const SliderAttributes& attributes() const { return d_attributes; }

    float value() const;

    void setCallback(SliderCallback cb) { d_callback = cb; }
};

}