#pragma once
#include "Types.h"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include <string>
#include <optional>

namespace Sprocket {
namespace Maths {

using ivec2 = glm::ivec2;
using ivec3 = glm::ivec3;
using ivec4 = glm::ivec4;

using quat = glm::quat;

constexpr quat identity = glm::identity<quat>();

// Matrix Modifiers
glm::mat4 Translate(const glm::mat4& matrix, const glm::vec3& translation);
glm::mat4 Scale(const glm::mat4& matrix, const glm::vec3& scales);
glm::mat4 Scale(const glm::mat4& matrix, float scale);

glm::mat4 Rotate(const glm::mat4& matrix, const glm::vec3& axis, float radians);
glm::mat4 Inverse(const glm::mat4& matrix);
glm::mat4 Transpose(const glm::mat4& matrix);

// Matrix Constructors
glm::mat4 Transform(const glm::vec3& position, const glm::quat& orientation, const glm::vec3& scale = {1.0f, 1.0f, 1.0f});
glm::mat4 Perspective(float aspectRatio, float fov, float nearPlane, float farPlane);
glm::mat4 View(const glm::vec3& position, float pitch, float yaw, float roll);
glm::mat4 LookAt(const glm::vec3& position, const glm::vec3& target, const glm::vec3& up = {0, 1, 0});
glm::mat4 Ortho(float left, float right, float bottom, float top);
glm::mat4 Ortho(float left, float right, float bottom, float top, float near, float far);
glm::mat4 Ortho(float width, float height, float length);

// Quaternion Modifiers
glm::quat Rotate(const glm::vec3& axis, float radians);
glm::quat Rotate(const glm::quat& quaternion, const glm::vec3& axis, float radians);
glm::quat Inverse(const glm::quat& quaternion);
glm::quat Normalise(const glm::quat& q);

quat LookAtQuat(const glm::vec3& position, const glm::vec3& target, const glm::vec3& up = {0, 1, 0});

// Given an orientation, find the directions of Forwards, Right and Up.
glm::vec3 Forwards(const glm::quat& q);
glm::vec3 Right(const glm::quat& q);
glm::vec3 Up(const glm::quat& q);

// Vector Modifiers
glm::vec3 Rotate(const glm::vec3& vector, const glm::vec3& axis, float radians);

// Conversions
glm::mat3 ToMat3(const glm::quat& q);
glm::mat4 ToMat4(const glm::quat& q);
glm::quat ToQuat(const glm::mat3& m);
glm::vec3 ToEuler(const glm::quat& q);

void Decompose(const glm::mat4& matrix, glm::vec3* position, glm::quat* orientation, glm::vec3* scale);

float* Cast(const glm::mat3& m);
float* Cast(const glm::mat4& m);

// Vector Maths
glm::vec3 Cross(const glm::vec3& lhs, const glm::vec3& rhs);
glm::vec3 GetTranslation(const glm::mat4& m);

float Distance(const glm::vec2& A, const glm::vec2& B);
float Distance(const glm::vec3& A, const glm::vec3& B);

float Length(const glm::vec3& v);
float Length(const glm::vec2& v);
float Length(const glm::ivec2& v);
float LengthSquare(const glm::vec3& v);

glm::vec3 Normalise(const glm::vec3& vec);

// Normalisation
glm::vec3 Interpolate(const glm::vec3& a, const glm::vec3& b, float delta);
glm::quat Interpolate(const glm::quat& a, const glm::quat& b, float delta);

// Trig
float Radians(float degrees);
float Degrees(float radians);

float Sind(float degrees);
float Cosd(float degrees);

// General Helpers
float Modulo(float val, float high);

// Returns the matrix but with the scale component removed.
glm::mat4 NoScale(const glm::mat4& matrix);

// Mouse Picking
glm::vec3 GetMouseRay(const glm::vec2& mousePos, u32 w, u32 h, const glm::mat4& view, const glm::mat4& proj);

// EXTRA OPERATIONS

// Appends a 1.0 to the vector, applies the transform, then strips the extra value.
// Has the action of taking a point and returning the point that the given transform
// moves it to.
glm::vec3 ApplyTransform(const glm::mat4& matrix, const glm::vec3& v);

// Printing
std::string ToString(const glm::vec3& v, const std::optional<int>& dp = {});
std::string ToString(float x, const std::optional<int>& dp = {});
std::string ToString(bool t);

}
}