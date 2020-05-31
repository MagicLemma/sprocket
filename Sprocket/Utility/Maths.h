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
mat4 Translate(const mat4& matrix, const vec3& translation);
mat4 Scale(const mat4& matrix, const vec3& scales);
mat4 Scale(const mat4& matrix, float scale);

mat4 Rotate(const mat4& matrix, const vec3& axis, float radians);
mat4 Inverse(const mat4& matrix);
mat4 Transpose(const mat4& matrix);

// Matrix Constructors
mat4 Transform(const vec3& position, const quat& orientation);
mat4 Perspective(float aspectRatio, float fov, float nearPlane, float farPlane);
mat4 View(const vec3& position, float pitch, float yaw, float roll);
mat4 LookAt(const vec3& position, const vec3& target, const vec3& up = {0, 1, 0});
mat4 Ortho(float left, float right, float bottom, float top);

// Quaternion Modifiers
quat Rotate(const vec3& axis, float radians);
quat Rotate(const quat& quaternion, const vec3& axis, float radians);
quat Inverse(const quat& quaternion);
quat Normalise(const quat& q);

// Conversions
mat3 ToMat3(const quat& q);
mat4 ToMat4(const quat& q);
quat ToQuat(const mat3& m);
vec3 ToEuler(const quat& q);

float* Cast(const mat3& m);
float* Cast(const mat4& m);

// Vector Maths
vec3 Cross(const vec3& lhs, const vec3& rhs);
vec3 GetTranslation(const mat4& m);

float Distance(const Maths::vec2& A, const Maths::vec2& B);

float Length(const vec3& v);
float LengthSquare(const vec3& v);

void Normalise(vec3& vec);

// Trig
float Radians(float degrees);
float Degrees(float radians);

float Sind(float degrees);
float Cosd(float degrees);

// General Helpers
void Clamp(float& value, float min, float max);

// Printing
std::string ToString(const vec3& v, const std::optional<int>& dp = {});
std::string ToString(float x, const std::optional<int>& dp = {});
std::string ToString(bool t);

}
}