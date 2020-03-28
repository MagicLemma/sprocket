#pragma once
#include "Maths.h"
#include "Font.h"

#include <string>

namespace Sprocket {

struct Text
{
    std::string message;
    Font        font;
    float       size;
    Maths::vec3 colour;
    Maths::vec2 position;

    float       lineWidth = 1000.0f;
};

}