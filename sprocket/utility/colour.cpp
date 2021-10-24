#include "colour.h"

#include <glm/glm.hpp>

namespace spkt {
namespace {

constexpr float normalise(int x)
{
    return static_cast<float>(x) / 256.0f;
}

}

glm::vec4 from_hex(int hex)
{
    float blue = normalise(hex & 0xff);
    hex /= 0x100;
    float green = normalise(hex & 0xff);
    hex /= 0x100;
    float red = normalise(hex & 0xff);
    return glm::vec4{red, green, blue, 1.0f};
}
    
}