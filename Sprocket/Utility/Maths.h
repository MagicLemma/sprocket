#pragma once
#include <glm/glm.hpp>

namespace Sprocket {
namespace Maths {

using mat4 = glm::mat4;
using vec2 = glm::vec2;
using vec3 = glm::vec3;
using vec4 = glm::vec4;

mat4 createTransformationMatrix(const vec3& translation,
                                const vec3& rotation,
                                float scale);

mat4 createProjectionMatrix(float aspectRatio,
                            float fov,
                            float nearPlane,
                            float farPlane);

mat4 createViewMatrix(const vec3& position,
                      float pitch,
                      float yaw,
                      float roll);

float radians(float degrees);
float sind(float degrees);
float cosd(float degrees);
void clamp(float& value, float min, float max);

vec3 cross(const vec3& lhs, const vec3& rhs);
void normalise(vec3& vec);



}
}