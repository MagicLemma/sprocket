#include "Printer.h"

#include <iomanip>

#include <fmt/core.h>

namespace Sprocket {
namespace Printer {

std::string PrintVec3(const glm::vec3& v, const std::optional<int>& dp)
{
    if (dp.has_value()) {
        return fmt::format("({:.{}f}, {:.{}f}, {:.{}f})", v.x, v.y, v.z, dp.value());
    }
    return fmt::format("({}, {}, {})", v.x, v.y, v.z);
}

std::string PrintFloat(float x, const std::optional<int>& dp)
{
    if (dp.has_value()) {
        return fmt::format("{:.{}f}", x, dp.value());
    }
    return fmt::format("{}", x);
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