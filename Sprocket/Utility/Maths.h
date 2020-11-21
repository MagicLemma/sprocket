#pragma once
#include "Types.h"

#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtc/quaternion.hpp>

#include <string>
#include <optional>

namespace Sprocket {
namespace Maths {

// Matrix Modifiers
glm::mat4 Rotate(const glm::mat4& matrix, const glm::vec3& axis, float radians);

// Matrix Constructors
glm::mat4 Transform(const glm::vec3& position, const glm::quat& orientation, const glm::vec3& scale = {1.0f, 1.0f, 1.0f});
glm::mat4 Perspective(float aspectRatio, float fov, float nearPlane, float farPlane);
glm::mat4 View(const glm::vec3& position, float pitch, float yaw, float roll);
glm::mat4 Ortho(float left, float right, float bottom, float top);
glm::mat4 Ortho(float left, float right, float bottom, float top, float near, float far);
glm::mat4 Ortho(float width, float height, float length);

// Quaternion Modifiers
glm::quat Rotate(const glm::vec3& axis, float radians);
glm::quat Rotate(const glm::quat& quaternion, const glm::vec3& axis, float radians);

glm::quat LookAtQuat(const glm::vec3& position, const glm::vec3& target, const glm::vec3& up = {0, 1, 0});

// Given an orientation, find the directions of Forwards, Right and Up.
glm::vec3 Forwards(const glm::quat& q);
glm::vec3 Right(const glm::quat& q);
glm::vec3 Up(const glm::quat& q);

// Vector Modifiers
glm::vec3 Rotate(const glm::vec3& vector, const glm::vec3& axis, float radians);

// Conversions
void Decompose(const glm::mat4& matrix, glm::vec3* position, glm::quat* orientation, glm::vec3* scale);

float* Cast(const glm::mat3& m);
float* Cast(const glm::mat4& m);

// Vector Maths
glm::vec3 Cross(const glm::vec3& lhs, const glm::vec3& rhs);
glm::vec3 GetTranslation(const glm::mat4& m);

// Normalisation
glm::vec3 Interpolate(const glm::vec3& a, const glm::vec3& b, float delta);
glm::quat Interpolate(const glm::quat& a, const glm::quat& b, float delta);

// Trig
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