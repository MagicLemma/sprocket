#include "Colour.h"
#include "Log.h"

#include <vector>

namespace spkt {
namespace {

constexpr float Normalize(int x)
{
    return (float)x/256;
}

}

glm::vec4 from_hex(int hexCode)
{
    int copy = hexCode;
    float blue = Normalize(copy & 0xff);
    copy /= 0x100;
    float green = Normalize(copy & 0xff);
    copy /= 0x100;
    float red = Normalize(copy & 0xff);
    return glm::vec4{red, green, blue, 1.0f};
}
    
}