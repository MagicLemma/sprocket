#include <string>
#include <optional>

#include <glm/glm.hpp>

namespace Sprocket {
namespace Printer {

std::string PrintVec3(const glm::vec3& v, const std::optional<int>& dp);
std::string PrintFloat(float x, const std::optional<int>& dp);
std::string PrintBool(bool t);

}
}