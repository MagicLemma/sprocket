#pragma once
#include <glm/glm.hpp>

namespace spkt {

glm::mat4 make_view(const glm::vec3& pos, const glm::quat& ori, float pitch);
glm::mat4 make_proj(float fov);
    
}