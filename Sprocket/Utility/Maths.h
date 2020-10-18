#pragma once
#include "Types.h"

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

using ivec2 = glm::ivec2;
using ivec3 = glm::ivec3;
using ivec4 = glm::ivec4;

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
mat4 Transform(const vec3& position, const quat& orientation, const vec3& scale = {1.0f, 1.0f, 1.0f});
mat4 Perspective(float aspectRatio, float fov, float nearPlane, float farPlane);
mat4 View(const vec3& position, float pitch, float yaw, float roll);
mat4 LookAt(const vec3& position, const vec3& target, const vec3& up = {0, 1, 0});
mat4 Ortho(float left, float right, float bottom, float top);
mat4 Ortho(float left, float right, float bottom, float top, float near, float far);
mat4 Ortho(float width, float height, float length);

// Quaternion Modifiers
quat Rotate(const vec3& axis, float radians);
quat Rotate(const quat& quaternion, const vec3& axis, float radians);
quat Inverse(const quat& quaternion);
quat Normalise(const quat& q);

quat LookAtQuat(const vec3& position, const vec3& target, const vec3& up = {0, 1, 0});

// Given an orientation, find the directions of Forwards, Right and Up.
vec3 Forwards(const quat& q);
vec3 Right(const quat& q);
vec3 Up(const quat& q);

// Vector Modifiers
vec3 Rotate(const vec3& vector, const vec3& axis, float radians);

// Conversions
mat3 ToMat3(const quat& q);
mat4 ToMat4(const quat& q);
quat ToQuat(const mat3& m);
vec3 ToEuler(const quat& q);

void Decompose(const mat4& matrix, vec3* position, quat* orientation, vec3* scale);

float* Cast(const mat3& m);
float* Cast(const mat4& m);

// Vector Maths
vec3 Cross(const vec3& lhs, const vec3& rhs);
vec3 GetTranslation(const mat4& m);

float Distance(const Maths::vec2& A, const Maths::vec2& B);
float Distance(const Maths::vec3& A, const Maths::vec3& B);

float Length(const vec3& v);
float Length(const vec2& v);
float Length(const ivec2& v);
float LengthSquare(const vec3& v);

void Normalise(vec3& vec);

// Normalisation
Maths::vec3 Interpolate(const Maths::vec3& a, const Maths::vec3& b, float delta);
Maths::quat Interpolate(const Maths::quat& a, const Maths::quat& b, float delta);

// Trig
float Radians(float degrees);
float Degrees(float radians);

float Sind(float degrees);
float Cosd(float degrees);

// General Helpers
void Clamp(float& value, float min, float max);
float Modulo(float val, float high);

// Returns the matrix but with the scale component removed.
Maths::mat4 NoScale(const Maths::mat4& matrix);

// Mouse Picking
vec3 GetMouseRay(const vec2& mousePos, u32 w, u32 h, const mat4& view, const mat4& proj);

// EXTRA OPERATIONS

// Appends a 1.0 to the vector, applies the transform, then strips the extra value.
// Has the action of taking a point and returning the point that the given transform
// moves it to.
Maths::vec3 ApplyTransform(const Maths::mat4& matrix, const Maths::vec3& v);

// Printing
std::string ToString(const vec3& v, const std::optional<int>& dp = {});
std::string ToString(float x, const std::optional<int>& dp = {});
std::string ToString(bool t);

}
}