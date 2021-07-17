#include "Colour.h"

#include <glm/glm.hpp>

namespace spkt {
namespace {

constexpr float Normalize(int x)
{
    return (float)x/256;
}

}

glm::vec4 from_hex(int hex)
{
    float blue = Normalize(hex & 0xff);
    hex /= 0x100;
    float green = Normalize(hex & 0xff);
    hex /= 0x100;
    float red = Normalize(hex & 0xff);
    return glm::vec4{red, green, blue, 1.0f};
}
    
}