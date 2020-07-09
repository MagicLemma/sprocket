#pragma once
#include "Maths.h"

#include <cstddef>

namespace Sprocket {

struct Glyph
{
    uint32_t codepoint = -1;

    std::size_t width  = 0;
    std::size_t height = 0;

    Maths::ivec2 offset = {0, 0};
    Maths::vec2 advance = {0.0f, 0.0f};
    Maths::vec4 texture = {0.0f, 0.0f, 0.0f, 0.0f};
};

}