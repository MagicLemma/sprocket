#pragma once
#include <glm/glm.hpp>

namespace Sprocket {
namespace Maths {

using mat3 = glm::mat3;
using mat4 = glm::mat4;
using vec2 = glm::vec2;
using vec3 = glm::vec3;
using vec4 = glm::vec4;

mat4 transform(const vec3& translation, const vec3& rotation, const vec3& scale);
mat4 transform(const vec3& translation, const vec3& rotation, float scale);
mat4 transform(const vec3& translation, const vec3& rotation);

mat4 translate(const mat4& matrix, const vec3& translation);
mat4 rotate(const mat4& matrix, const vec3& axis, float radians);

mat4 scale(const mat4& matrix, const vec3& scales);
mat4 scale(const mat4& matrix, float scale);

mat4 perspective(float aspectRatio,
                 float fov,
                 float nearPlane,
                 float farPlane);

mat4 createViewMatrix(const vec3& position,
                      float pitch,
                      float yaw,
                      float roll);

mat4 lookAt(const vec3& position,
            const vec3& target,
            const vec3& up = vec3{0.0f, 1.0f, 0.0f});

mat4 ortho(float left, float right,
           float bottom, float top);

mat4 inverse(const mat4& matrix);
mat4 transpose(const mat4& matrix);

float radians(float degrees);
float degrees(float radians);

float sind(float degrees);
float cosd(float degrees);
void clamp(float& value, float min, float max);

vec3 cross(const vec3& lhs, const vec3& rhs);
void normalise(vec3& vec);

float distance(const Maths::vec2& A, const Maths::vec2& B);

vec4 toQuaternion(const vec3& euler);
vec3 toEuler(const vec4& quaternion);

vec3 getTranslation(const mat4& transform);
    // Extract the translation out of a transform matrix.
}
}