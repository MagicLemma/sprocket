#include "Printer.h"

#include <sstream>
#include <iomanip>

namespace Sprocket {
namespace Printer {

std::string PrintVec3(const glm::vec3& v, const std::optional<int>& dp)
{
    std::stringstream ss;
    if (dp.has_value()) {
        ss << std::fixed << std::setprecision(dp.value());
    }
    ss << "(" << v.x << ", " << v.y << ", " << v.z << ")";
    return ss.str();
}

std::string PrintFloat(float x, const std::optional<int>& dp)
{
    std::stringstream ss;
    if (dp.has_value()) {
        ss << std::fixed << std::setprecision(dp.value());
    }
    ss << x;
    return ss.str();
}

std::string PrintBool(bool t)
{
    if (t) {
        return "True";
    }
    return "False";
}


}
}