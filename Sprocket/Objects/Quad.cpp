#include "Quad.h"

#include <cmath>

namespace Sprocket {

bool ContainsPoint(const Quad& quad, const Maths::vec2& point)
{
    return point.x >= quad.position.x
        && point.x <= quad.position.x + quad.width
        && point.y >= quad.position.y
        && point.y <= quad.position.y + quad.height;
}

}