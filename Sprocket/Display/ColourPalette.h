#pragma once
#include "Widget.h"
#include "Quad.h"
#include "Model2D.h"
#include "Maths.h"

#include <functional>
#include <vector>

namespace Sprocket {

using ColourPaletteCallback = std::function<void(const Maths::vec3&)>;

class ColourPalette : public Widget
{
    static constexpr float s_border = 10.0f;

    Maths::vec3 d_paletteColour;
    Maths::vec3 d_overallColour;

    // Colour Picker
    Quad                 d_paletteQuad;
    std::vector<Model2D> d_segments;
    Maths::vec2          d_palettePickerPosition;
    Quad                 d_palettePicker;

    // Black/White Picker
    Quad    d_blackWhiteQuad;
    Model2D d_top;
    Model2D d_bottom;
    Quad    d_sliderPicker;

    // Bottom Display
    Quad d_bar;

    bool d_movingPalettePicker = false;
    bool d_movingSliderPicker = false;

    ColourPaletteCallback d_callback = [](const Maths::vec3&){};

    void updateImpl(DisplayRenderer* renderer) override;
    void handleEventImpl(Event& event) override;

public:
    ColourPalette(float width, float height);

    void setCallback(ColourPaletteCallback cb) { d_callback = cb; }
};

}