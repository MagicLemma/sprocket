#pragma once
#include "Maths.h"
#include "Font.h"

#include <string>

namespace Sprocket {

struct Text
{
    std::string message;
    Font        font = Font::ARIAL;
    float       size = 24.0f;
    Maths::vec3 colour = {1.0, 1.0, 1.0};
    Maths::vec2 position = {0.0, 0.0};

    float       lineWidth = 1000.0f;
};

}