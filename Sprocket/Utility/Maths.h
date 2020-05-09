#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <string>
#include <optional>

namespace Sprocket {
namespace Maths {

using mat3 = glm::mat3;
using mat4 = glm::mat4;

using vec2 = glm::vec2;
using vec3 = glm::vec3;
using vec4 = glm::vec4;

using quat = glm::quat;

constexpr quat identity = glm::identity<quat>();

// Matrix Modifiers
mat4 translate(const mat4& matrix, const vec3& translation);
mat4 scale(const mat4& matrix, const vec3& scales);
mat4 scale(const mat4& matrix, float scale);

mat4 rotate(const mat4& matrix, const vec3& axis, float radians);
mat4 inverse(const mat4& matrix);

// Matrix Constructors
mat4 transform(const vec3& position, const quat& orientation);
mat4 perspective(float aspectRatio, float fov, float nearPlane, float farPlane);
mat4 view(const vec3& position, float pitch, float yaw, float roll);
mat4 lookAt(const vec3& position, const vec3& target, const vec3& up = {0, 1, 0});
mat4 ortho(float left, float right, float bottom, float top);

// Quaternion Modifiers
quat rotate(const vec3& axis, float radians);
quat rotate(const quat& quaternion, const vec3& axis, float radians);
quat inverse(const quat& quaternion);

// Conversions
mat3 toMat3(const quat& q);
quat toQuat(const mat3& m);

// Vector Maths
vec3 cross(const vec3& lhs, const vec3& rhs);

float distance(const Maths::vec2& A, const Maths::vec2& B);

float length(const vec3& v);
float lengthSquare(const vec3& v);

void normalise(vec3& vec);

// Trig
float radians(float degrees);
float degrees(float radians);

float sind(float degrees);
float cosd(float degrees);

// General Helpers
void clamp(float& value, float min, float max);

// Printing
std::string toString(const vec3& v, const std::optional<int>& dp = {});

}
}