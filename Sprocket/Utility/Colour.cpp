#include "Colour.h"
#include "Log.h"

#include <vector>

namespace Sprocket {
namespace {

constexpr float Normalize(int x)
{
    return (float)x/256;
}

}

Maths::vec4 FromHex(int hexCode)
{
    int copy = hexCode;
    float blue = Normalize(copy & 0xff);
    copy /= 0x100;
    float green = Normalize(copy & 0xff);
    copy /= 0x100;
    float red = Normalize(copy & 0xff);
    return Maths::vec4{red, green, blue, 1.0f};
}
    
}