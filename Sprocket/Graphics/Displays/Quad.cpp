#include "Quad.h"

#include <cmath>

namespace Sprocket {

Quad::Quad(const Maths::vec2& topLeft, float width, float height)
    : d_topLeft(topLeft)
    , d_width(width)
    , d_height(height)
    , d_colour({1.0f, 1.0f, 1.0f})
{
}

Maths::vec2 Quad::centre() const {
    return {
        d_topLeft.x + d_width/2,
        d_topLeft.y + d_height/2
    };
}

bool Quad::containsPoint(float x, float y) const
{
    return x >= d_topLeft.x
        && x <= d_topLeft.x + d_width
        && y >= d_topLeft.y
        && y <= d_topLeft.y + d_height;
}

bool Quad::containsPoint(const Maths::vec2& pos) const
{
    return containsPoint(pos.x, pos.y);
}

}