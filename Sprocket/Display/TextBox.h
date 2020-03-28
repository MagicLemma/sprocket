#pragma once
#include "Widget.h"
#include "Quad.h"
#include "Text.h"

namespace Sprocket {

class TextBox : public Widget
{
    Quad        d_background;
    Text        d_text;

    void updateImpl(Window* window) override;
    bool handleEventImpl(Window* window, const Event& event) override;
    void drawImpl(DisplayRenderer* renderer) const override;

public:
    TextBox(float width, float height, const std::string& message);

    Quad& background() { return d_background; }
    const Quad& background() const { return d_background; }

    Text& text() { return d_text; }
    const Text& text() const { return d_text; }
};

}