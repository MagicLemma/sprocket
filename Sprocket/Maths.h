#pragma once
#include <glm/glm.hpp>

namespace Sprocket {
namespace Maths {

glm::mat4 createTransformationMatrix(const glm::vec3& translation,
                                     float rotation_x,
                                     float rotation_y,
                                     float rotation_z,
                                     float scale);


}
}