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

    float Left() const;
    float Right() const;
        // Maximum left and right positions of the picker in
        // Local Coordinates.

    void OnUpdateImpl(DisplayRenderer* renderer) override;
    void OnEventImpl(Event& event) override;
    
public:
    Slider(float width, float height);

    Quad& Bar() { return d_bar; }
    Quad& Picker() { return d_picker; }
    const Quad& Bar() const { return d_bar; }
    const Quad& Picker() const { return d_picker; }

    float Value() const;
    void SetValue(float val);

    void SetCallback(SliderCallback cb) { d_callback = cb; }
};

}