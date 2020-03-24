#pragma once
#include "Maths.h"
#include "Texture.h"

#include <variant>

namespace Sprocket {

using Colour = Maths::vec3;
using Skin = std::variant<Colour, Texture>;

struct Quad
{
    Maths::vec2 position;
    float       width;
    float       height;   
};

struct VisualQuad
{
    Quad  body;
    Skin  skin;
    float opacity = 1.0f;
    float roundness = 0.0f;
};

bool containsPoint(const Quad& quad, const Maths::vec2& point);
bool containsPoint(const VisualQuad& quad, const Maths::vec2& point);

}