#pragma once
#include "Widget.h"
#include "Quad.h"
#include "Text.h"

namespace Sprocket {

class TextBox : public Widget
{
    Text        d_text;

    void OnUpdateImpl(DisplayRenderer* renderer) override;
    void OnEventImpl(Event& event) override {};

public:
    TextBox(float width, float height, const std::string& message);

    Text& TextData() { return d_text; }
    const Text& TextData() const { return d_text; }
};

}