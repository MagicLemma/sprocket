#include "Maths.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/trigonometric.hpp>

namespace Sprocket {
namespace Maths {

glm::mat4 createTransformationMatrix(const glm::vec3& translation,
                                     float rotation_x,
                                     float rotation_y,
                                     float rotation_z,
                                     float scale)
{
    glm::mat4 matrix(1.0);
    glm::translate(matrix, translation);
    glm::rotate(matrix, glm::radians(rotation_x), glm::vec3(1, 0, 0));
    glm::rotate(matrix, glm::radians(rotation_y), glm::vec3(0, 1, 0));
    glm::rotate(matrix, glm::radians(rotation_z), glm::vec3(0, 0, 1));
    glm::scale(matrix, glm::vec3(scale, scale, scale));
    return matrix;
}

}
}