#include "Utility/Maths.h"

#include <cmath>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/trigonometric.hpp>

namespace Sprocket {
namespace Maths {

mat4 createTransformationMatrix(const vec3& translation,
                                const vec3& rotation,
                                float scale)
{
    mat4 matrix(1.0);
    matrix = glm::translate(matrix, translation);
    matrix = glm::rotate(matrix, radians(rotation.x), vec3(1, 0, 0));
    matrix = glm::rotate(matrix, radians(rotation.y), vec3(0, 1, 0));
    matrix = glm::rotate(matrix, radians(rotation.z), vec3(0, 0, 1));
    matrix = glm::scale(matrix, vec3(scale, scale, scale));
    return matrix;
}

mat4 createProjectionMatrix(float aspectRatio,
                            float fov,
                            float nearPlane,
                            float farPlane)
{
    mat4 matrix(1.0);
    float xScale = (1.0f / std::tan(radians(fov / 2.0f)));
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

mat4 createViewMatrix(const vec3& position,
                      float pitch,
                      float yaw,
                      float roll)
{
    mat4 matrix(1.0);
    matrix = glm::rotate(matrix, radians(pitch), vec3(1, 0, 0));
    matrix = glm::rotate(matrix, radians(yaw), vec3(0, 1, 0));
    matrix = glm::rotate(matrix, radians(roll), vec3(0, 0, 1));
    matrix = glm::translate(matrix, -position);
    return matrix;
}

float radians(float degrees)
{
    return glm::radians(degrees);
}

}
}