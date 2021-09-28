#include "camera.h"

#include <Sprocket/Graphics/Viewport.h>
#include <Sprocket/Core/log.h>
#include <Sprocket/Utility/Maths.h>

#include <glm/glm.hpp>

#include <cassert>

namespace spkt {

glm::mat4 spkt::make_view(const spkt::registry& registry, spkt::entity entity)
{
    assert((registry.has_all<Transform3DComponent, Camera3DComponent>(entity)));
    auto [tr, cc] = registry.get_all<Transform3DComponent, Camera3DComponent>(entity);
    return make_view(tr.position, tr.orientation, cc.pitch);
}

glm::mat4 make_view(const glm::vec3& pos, const glm::quat& ori, float pitch)
{
    return glm::inverse(Maths::Transform(
        pos, ori * glm::rotate(glm::identity<glm::quat>(), pitch, {1, 0, 0})
    ));
}

glm::mat4 make_proj(const spkt::registry& registry, spkt::entity entity)
{
    assert(registry.has<Camera3DComponent>(entity));
    return make_proj(glm::radians(registry.get<Camera3DComponent>(entity).fov));
}

glm::mat4 make_proj(float fov)
{
    return glm::perspective(fov, Viewport::CurrentAspectRatio(), 0.01f, 1000.0f);
}

}