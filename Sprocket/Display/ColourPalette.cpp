#include "ColourPalette.h"
#include "MouseEvent.h"
#include "Log.h"

namespace Sprocket {
namespace {

Model2D getSegment(const Maths::vec3& left, const Maths::vec3& right)
{
    return {{
        {{1.0f, 1.0f}, {1.0f, 1.0f}, {0.5f, 0.5f, 0.5f}},
        {{1.0f, 0.0f}, {1.0f, 0.0f}, right},
        {{0.0f, 1.0f}, {0.0f, 1.0f}, {0.5f, 0.5f, 0.5f}},
        {{0.0f, 0.0f}, {0.0f, 0.0f}, left}
    }};
}

Maths::vec3 interpolateColour(const Maths::vec2& pos)
{
    Maths::vec3 ret;
    if (0 < pos.x && pos.x < 1.0f / 6) {
        float horiz = pos.x * 6.0f;
        ret = (1 - horiz) * Maths::vec3{1.0, 0.0, 0.0} + horiz * Maths::vec3{1.0, 1.0, 0.0};
    }
    else if (1.0f / 6 < pos.x && pos.x < 2.0f / 6) {
        float horiz = pos.x * 6.0f - 1;
        ret = (1 - horiz) * Maths::vec3{1.0, 1.0, 0.0} + horiz * Maths::vec3{0.0, 1.0, 0.0};
    }
    else if (2.0f / 6 < pos.x && pos.x < 3.0f / 6) {
       float horiz = pos.x * 6.0f - 2;
       ret = (1 - horiz) * Maths::vec3{0.0, 1.0, 0.0} + horiz * Maths::vec3{0.0, 1.0, 1.0};
    }
    else if (3.0f / 6 < pos.x && pos.x < 4.0f / 6) {
        float horiz = pos.x * 6.0f - 3;
        ret = (1 - horiz) * Maths::vec3{0.0, 1.0, 1.0} + horiz * Maths::vec3{0.0, 0.0, 1.0};
    }
    else if (4.0f / 6 < pos.x && pos.x < 5.0f / 6) {
        float horiz = pos.x * 6.0f - 4;
        ret = (1 - horiz) * Maths::vec3{0.0, 0.0, 1.0} + horiz * Maths::vec3{1.0, 0.0, 1.0};
    }
    else {
        float horiz = pos.x * 6.0f - 5;
        ret = (1 - horiz) * Maths::vec3{1.0, 0.0, 1.0} + horiz * Maths::vec3{1.0, 0.0, 0.0};
    }

    return (1 - pos.y) * ret + (pos.y) * Maths::vec3{0.5, 0.5, 0.5};
}

}

ColourPalette::ColourPalette(float width, float height)
    : Widget(width, height)
    , d_paletteColour({0.0, 1.0, 1.0})
    , d_overallColour({1.0, 0.0, 0.0})
    , d_paletteQuad({
        {s_border, s_border},
        6 * (width - 3 * s_border) / 7,
        6 * (width - 3 * s_border) / 7
    })
    , d_segments({
        getSegment({1.0, 0.0, 0.0}, {1.0, 1.0, 0.0}),
        getSegment({1.0, 1.0, 0.0}, {0.0, 1.0, 0.0}),
        getSegment({0.0, 1.0, 0.0}, {0.0, 1.0, 1.0}),
        getSegment({0.0, 1.0, 1.0}, {0.0, 0.0, 1.0}),
        getSegment({0.0, 0.0, 1.0}, {1.0, 0.0, 1.0}),
        getSegment({1.0, 0.0, 1.0}, {1.0, 0.0, 0.0})
    })
    , d_palettePickerPosition({0.0, 0.0})
    , d_palettePicker({
        {s_border / 2, s_border / 2},
        10.0f,
        10.0f
    })
    , d_blackWhiteQuad({
        {2 * s_border + 6 * (width - 3 * s_border) / 7, s_border},
        (width - 3 * s_border) / 7,
        6 * (width - 3 * s_border) / 7
    })
    , d_top({{
        {{1.0f, 1.0f}, {1.0f, 1.0f}, {-1.0, 1.0, 0.0}},
        {{1.0f, 0.0f}, {1.0f, 0.0f}, {1.0, 1.0, 1.0}},
        {{0.0f, 1.0f}, {0.0f, 1.0f}, {-1.0, 1.0, 0.0}},
        {{0.0f, 0.0f}, {0.0f, 0.0f}, {1.0, 1.0, 1.0}}
    }})
    , d_bottom({{
        {{1.0f, 1.0f}, {1.0f, 1.0f}, {0.0, 0.0, 0.0}},
        {{1.0f, 0.0f}, {1.0f, 0.0f}, {-1.0, 1.0, 0.0}},
        {{0.0f, 1.0f}, {0.0f, 1.0f}, {0.0, 0.0, 0.0}},
        {{0.0f, 0.0f}, {0.0f, 0.0f}, {-1.0, 1.0, 0.0}}
    }})
    , d_sliderPicker({
        {2 * s_border + 6 * (width - 3 * s_border) / 7 - (s_border / 2), s_border},
        (width - 3 * s_border) / 7 + s_border,
        (6 * (width - 3 * s_border) / 7) / 50
    })
    , d_bar({
        {s_border, 2 * s_border + 6 * (width - 3 * s_border) / 7},
        width - 2 * s_border,
        (width - 3 * s_border) / 7
    })
{
    d_palettePicker.roundness = 1.0f;
}
    
void ColourPalette::updateImpl(DisplayRenderer* renderer)
{
    if (d_movingPalettePicker) {
        Maths::vec2 newPosition = toLocalCoords(d_mouse.getMousePos());
        Maths::clamp(newPosition.x, d_paletteQuad.position.x, d_paletteQuad.position.x + d_paletteQuad.width);
        Maths::clamp(newPosition.y, d_paletteQuad.position.y, d_paletteQuad.position.y + d_paletteQuad.height);
        d_palettePicker.position = newPosition - Maths::vec2{d_palettePicker.width/2, d_palettePicker.height/2};

        d_paletteColour = interpolateColour({
            newPosition.x / d_paletteQuad.width,
            newPosition.y / d_paletteQuad.height
        });
    }

    if (d_movingSliderPicker) {
        float newVertPos = toLocalCoords(d_mouse.getMousePos()).y - d_sliderPicker.height/2;
        Maths::clamp(newVertPos, d_blackWhiteQuad.position.y - d_sliderPicker.height/2, d_blackWhiteQuad.position.y + d_blackWhiteQuad.height - d_sliderPicker.height/2);
        d_sliderPicker.position.y = newVertPos;
    }

    if (d_movingPalettePicker || d_movingSliderPicker) { // Update the slider and bar if either picker is updating
        float ratio = (d_sliderPicker.position.y + d_sliderPicker.height/2) / (d_blackWhiteQuad.height + d_sliderPicker.height/2);
        if (ratio < 0.5f) {
            ratio = 2 * ratio;
            d_overallColour = (1 - ratio) * Maths::vec3(1.0, 1.0, 1.0) + ratio * d_paletteColour;
        }
        else {
            ratio = 2 * ratio - 1;
            d_overallColour = (1 - ratio) * d_paletteColour + ratio * Maths::vec3(0.0, 0.0, 0.0);
        }
        d_bar.colour = d_overallColour;
        d_callback(d_overallColour);
    }

    Quad quad = d_paletteQuad;
    quad.width /= d_segments.size();
    for (const auto& segment : d_segments) {
        renderer->draw(toScreenCoords(quad), segment);
        quad.position.x += quad.width;
    }


    quad = d_blackWhiteQuad;
    quad.height /= 2;
    quad.colour = d_paletteColour;
    renderer->draw(toScreenCoords(quad), d_top);
    quad.position.y += quad.height;
    renderer->draw(toScreenCoords(quad), d_bottom);

    renderer->draw(toScreenCoords(d_bar));

    renderer->draw(toScreenCoords(d_palettePicker));
    renderer->draw(toScreenCoords(d_sliderPicker));
}

void ColourPalette::handleEventImpl(Event& event)
{
    if (containsPoint(d_paletteQuad, toLocalCoords(d_mouse.getMousePos()))) {
        if (auto e = event.as<MouseButtonPressedEvent>()) {
            if (!e->isConsumed() && e->button() == Mouse::LEFT) {
                d_movingPalettePicker = true;
                Maths::vec2 newPosition = toLocalCoords(d_mouse.getMousePos());
                Maths::clamp(newPosition.x, d_paletteQuad.position.x, d_paletteQuad.position.x + d_paletteQuad.width);
                Maths::clamp(newPosition.y, d_paletteQuad.position.y, d_paletteQuad.position.y + d_paletteQuad.height);
                d_palettePicker.position = newPosition - Maths::vec2{d_palettePicker.width/2, d_palettePicker.height/2};

                d_paletteColour = interpolateColour({
                    newPosition.x / d_paletteQuad.width,
                    newPosition.y / d_paletteQuad.height
                });
                e->consume();
            }
        }
    } else if (containsPoint(d_blackWhiteQuad, toLocalCoords(d_mouse.getMousePos()))) {
        if (auto e = event.as<MouseButtonPressedEvent>()) {
            if (!e->isConsumed() && e->button() == Mouse::LEFT) {
                d_movingSliderPicker = true;
                float newVertPos = toLocalCoords(d_mouse.getMousePos()).y - d_sliderPicker.height/2;
                Maths::clamp(newVertPos, d_blackWhiteQuad.position.y - d_sliderPicker.height/2, d_blackWhiteQuad.position.y + d_blackWhiteQuad.height - d_sliderPicker.height/2);
                d_sliderPicker.position.y = newVertPos;
                e->consume();
            }
        }       
    }

    if (auto e = event.as<MouseButtonReleasedEvent>()) {
        if (!e->isConsumed() && e->button() == Mouse::LEFT) {
            d_movingPalettePicker = false;
            d_movingSliderPicker = false;
        }
    }
}

}