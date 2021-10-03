#include "scene_utils.h"
#include <Anvil/ecs/ecs.h>

#include <Sprocket/Graphics/camera.h>

#include <glm/glm.hpp>

#include <utility>

namespace anvil {

std::pair<glm::mat4, glm::mat4> get_proj_view_matrices(
    const spkt::registry& registry, spkt::entity camera)
{
    auto [tc, cc] = registry.get_all<spkt::Transform3DComponent, spkt::Camera3DComponent>(camera);
    return {
        spkt::make_proj(cc.fov),
        spkt::make_view(tc.position, tc.orientation, cc.pitch)
    };
}

}