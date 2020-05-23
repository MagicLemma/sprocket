#pragma once
#include "Widget.h"
#include "Quad.h"
#include "Text.h"

namespace Sprocket {

class TextBox : public Widget
{
    Text        d_text;

    void updateImpl(Window* window) override {};
    void handleEventImpl(Window* window, Event& event) override {};
    void drawImpl(DisplayRenderer* renderer) const override;

public:
    TextBox(float width, float height, const std::string& message);

    Text& text() { return d_text; }
    const Text& text() const { return d_text; }
};

}