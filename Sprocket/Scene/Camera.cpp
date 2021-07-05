#include "Camera.h"
#include "Log.h"
#include "Components.h"
#include "Viewport.h"

namespace spkt {

glm::mat4 spkt::make_view(apx::registry& registry, apx::entity entity)
{
    if (!registry.has<Transform3DComponent>(entity)) {
        log::error("Camera has no transform component!");
        return glm::mat4{1.0};
    }

    auto tr = registry.get<Transform3DComponent>(entity);

    if (registry.has<Camera3DComponent>(entity)) {
        const auto& c = registry.get<Camera3DComponent>(entity);
        tr.orientation *= glm::rotate(glm::identity<glm::quat>(), c.pitch, {1, 0, 0});
    }

    return glm::inverse(Maths::Transform(tr.position, tr.orientation));   
}

glm::mat4 make_proj(apx::registry& registry, apx::entity entity)
{
    float fov = glm::radians(70.0f);

    if (registry.has<Camera3DComponent>(entity)) {
        fov = glm::radians(registry.get<Camera3DComponent>(entity).fov);
    }

    return glm::perspective(fov, Viewport::CurrentAspectRatio(), 0.01f, 1000.0f);
}

}