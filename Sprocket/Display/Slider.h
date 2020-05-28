#pragma once
#include "Quad.h"
#include "Widget.h"
#include "Window.h"
#include "Event.h"

#include <functional>

namespace Sprocket {

using SliderCallback = std::function<void(float)>;

class Slider : public Widget
{
    Quad d_bar;
    Quad d_picker;
    
    bool d_updating;

    SliderCallback d_callback = [](float){};

    float left() const;
    float right() const;
        // Maximum left and right positions of the picker in
        // Local Coordinates.

    void updateImpl(DisplayRenderer* renderer) override;
    void handleEventImpl(Event& event) override;
    
public:
    Slider(float width, float height);

    Quad& bar() { return d_bar; }
    Quad& picker() { return d_picker; }
    const Quad& bar() const { return d_bar; }
    const Quad& picker() const { return d_picker; }

    float value() const;
    void setValue(float val);

    void setCallback(SliderCallback cb) { d_callback = cb; }
};

}