#pragma once
#include "Widget.h"
#include "Quad.h"
#include "Text.h"

namespace Sprocket {

struct TextBoxAttributes
{
    Maths::vec3 backgroundColour = {0.0f, 0.0f, 0.0f};
    Maths::vec3 textColour = {1.0f, 1.0f, 1.0f};
    float textSize = 24;
    Font textFont = Font::CALIBRI;
    Maths::vec2 textOffset = {10.0, 10.0};
};

class TextBox : public Widget
{
    Quad        d_background;
    QuadVisuals d_backgroundVisuals;

    Text        d_text;

    TextBoxAttributes d_attributes;

    void updateImpl(Window* window) override;
    bool handleEventImpl(Window* window, const Event& event) override;
    void drawImpl(DisplayRenderer* renderer) const override;

public:
    TextBox(float width,
            float height,
            const std::string& message,
            const TextBoxAttributes& attrs = {});

    Quad& background() { return d_background; }
    const Quad& background() const { return d_background; }

    QuadVisuals& backgroundVisuals() { return d_backgroundVisuals; }
    const QuadVisuals& backgroundVisuals() const { return d_backgroundVisuals; }

    Text& text() { return d_text; }
    const Text& text() const { return d_text; }

    const TextBoxAttributes& attributes() const { return d_attributes; }
};

}