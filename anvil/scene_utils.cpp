#include "scene_utils.h"
#include <anvil/ecs/ecs.h>

#include <sprocket/graphics/camera.h>

#include <glm/glm.hpp>

#include <utility>

namespace anvil {

std::pair<glm::mat4, glm::mat4> get_proj_view_matrices(
    const anvil::registry& registry, anvil::entity camera)
{
    auto [tc, cc] = registry.get_all<anvil::Transform3DComponent, anvil::Camera3DComponent>(camera);
    return {
        spkt::make_proj(cc.fov),
        spkt::make_view(tc.position, tc.orientation, cc.pitch)
    };
}

}