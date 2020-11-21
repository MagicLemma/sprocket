#pragma once
#include "Types.h"

#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <optional>

namespace Sprocket {
namespace Maths {

// Matrix Constructors
glm::mat4 Transform(const glm::vec3& position, const glm::quat& orientation, const glm::vec3& scale = {1.0f, 1.0f, 1.0f});

// Given an orientation, find the directions of Forwards, Right and Up.
glm::vec3 Forwards(const glm::quat& q);
glm::vec3 Right(const glm::quat& q);
glm::vec3 Up(const glm::quat& q);

void Decompose(const glm::mat4& matrix, glm::vec3* position, glm::quat* orientation, glm::vec3* scale);
glm::vec3 GetTranslation(const glm::mat4& m);
float Modulo(float val, float high);
glm::mat4 NoScale(const glm::mat4& matrix);
glm::vec3 GetMouseRay(const glm::vec2& mousePos, u32 w, u32 h, const glm::mat4& view, const glm::mat4& proj);
glm::vec3 ApplyTransform(const glm::mat4& matrix, const glm::vec3& v);

}
}