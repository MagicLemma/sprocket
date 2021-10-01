#pragma once
#include <Sprocket/Scene/ecs.h>

#include <glm/glm.hpp>

#include <utility>

namespace anvil {

std::pair<glm::mat4, glm::mat4> get_proj_view_matrices(
    const spkt::registry& registry, spkt::entity camera
);

}