#include "Material.h"

namespace Sprocket {

bool operator==(const Material& lhs, const Material& rhs)
{
    return lhs.texture == rhs.texture &&
           lhs.reflectivity == rhs.reflectivity &&
           lhs.shineDamper == rhs.shineDamper;
}

}