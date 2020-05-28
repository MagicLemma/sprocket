#pragma once
#include "Widget.h"
#include "Quad.h"
#include "Maths.h"
#include "Texture.h"

#include <string>

namespace Sprocket {

class Image : public Widget
{
    void updateImpl(DisplayRenderer* renderer) override {};
    void handleEventImpl(Event& event) override {};

public:
    Image(const Texture& texture);
};

}