#pragma once
#include <glm/glm.hpp>

namespace Sprocket {
namespace Maths {

glm::mat4 createTransformationMatrix(const glm::vec3& translation,
                                     const glm::vec3& rotation,
                                     float scale);

glm::mat4 createProjectionMatrix(float aspectRatio,
                                 float fov,
                                 float nearPlane,
                                 float farPlane);


}
}