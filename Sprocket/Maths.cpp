#include "Maths.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/trigonometric.hpp>

namespace Sprocket {
namespace Maths {

glm::mat4 createTransformationMatrix(const glm::vec3& translation,
                                     const glm::vec3& rotation,
                                     float scale)
{
    glm::mat4 matrix(1.0);
    matrix = glm::translate(matrix, translation);
    matrix = glm::rotate(matrix, glm::radians(rotation.x), glm::vec3(1, 0, 0));
    matrix = glm::rotate(matrix, glm::radians(rotation.y), glm::vec3(0, 1, 0));
    matrix = glm::rotate(matrix, glm::radians(rotation.z), glm::vec3(0, 0, 1));
    matrix = glm::scale(matrix, glm::vec3(scale, scale, scale));
    return matrix;
}

}
}