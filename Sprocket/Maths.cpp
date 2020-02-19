#include "Maths.h"
#include "Log.h"

#include <cmath>

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

glm::mat4 createProjectionMatrix(float aspectRatio,
                                 float fov,
                                 float nearPlane,
                                 float farPlane)
{
    glm::mat4 matrix(1.0);
    float xScale = (1.0f / std::tan(glm::radians(fov / 2.0f)));
    float yScale = xScale * aspectRatio;
    float frustumLength = farPlane - nearPlane;

    matrix[0][0] = xScale;
    matrix[1][1] = yScale;
    matrix[2][2] = -((farPlane + nearPlane) / frustumLength);
    matrix[2][3] = -1;
    matrix[3][2] = - ((2 * nearPlane * farPlane) / frustumLength);
    matrix[3][3] = 0;
    return matrix;   
}

glm::mat4 createViewMatrix(const glm::vec3& position,
                           float pitch,
                           float yaw,
                           float roll)
{
    glm::mat4 matrix(1.0);
    matrix = glm::rotate(matrix, glm::radians(pitch), glm::vec3(1, 0, 0));
    matrix = glm::rotate(matrix, glm::radians(yaw), glm::vec3(0, 1, 0));
    matrix = glm::rotate(matrix, glm::radians(roll), glm::vec3(0, 0, 1));
    matrix = glm::translate(matrix, -position);
    return matrix;
}

}
}