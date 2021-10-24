#pragma once
#include <glm/glm.hpp>

#include <cstddef>

namespace spkt {

struct glyph
{
    std::uint32_t codepoint = -1;

    std::size_t width  = 0;
    std::size_t height = 0;

    glm::ivec2 offset = {0, 0};
    glm::vec2 advance = {0.0f, 0.0f};
    glm::vec4 texture = {0.0f, 0.0f, 0.0f, 0.0f};
};

}