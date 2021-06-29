#include "Camera.h"
#include "Log.h"
#include "Components.h"
#include "Viewport.h"

namespace spkt {

glm::mat4 spkt::make_view(spkt::entity entity)
{
    if (!entity.has<Transform3DComponent>()) {
        log::error("Camera has no transform component!");
        return glm::mat4{1.0};
    }

    auto tr = entity.get<Transform3DComponent>();

    if (entity.has<Camera3DComponent>()) {
        const auto& c = entity.get<Camera3DComponent>();
        tr.orientation *= glm::rotate(glm::identity<glm::quat>(), c.pitch, {1, 0, 0});
    }

    return glm::inverse(Maths::Transform(tr.position, tr.orientation));   
}

glm::mat4 make_proj(spkt::entity entity)
{
    float fov = glm::radians(70.0f);

    if (entity.has<Camera3DComponent>()) {
        fov = glm::radians(entity.get<Camera3DComponent>().fov);
    }

    return glm::perspective(fov, Viewport::CurrentAspectRatio(), 0.01f, 1000.0f);
}

}