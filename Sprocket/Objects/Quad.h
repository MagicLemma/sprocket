#pragma once
#include "Maths.h"
#include "Texture.h"

#include <variant>
#include <utility>
#include <optional>

namespace Sprocket {

using Colour = Maths::vec3;

struct Quad
{
    // Dimensions
    Maths::vec2 position;
    float       width;
    float       height;

    // Visuals
    std::optional<Texture> texture = {};
        // If no texture is provided, a plain white texture is used.
        
    Colour colour = Colour{1.0, 1.0, 1.0};
    float opacity = 1.0f;
    float roundness = 0.0f;
    bool greyscale = false; 
};

bool containsPoint(const Quad& quad, const Maths::vec2& point);

}