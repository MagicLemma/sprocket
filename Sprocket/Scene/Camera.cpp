#include "Camera.h"
#include "Log.h"
#include "Components.h"
#include "Viewport.h"

namespace Sprocket {

glm::mat4 MakeView(const ecs::Entity& entity)
{
    if (!entity.Has<Transform3DComponent>()) {
        log::error("Camera has no transform component!");
        return glm::mat4{1.0};
    }

    auto tr = entity.Get<Transform3DComponent>();

    if (entity.Has<Camera3DComponent>()) {
        const auto& c = entity.Get<Camera3DComponent>();
        tr.orientation *= glm::rotate(glm::identity<glm::quat>(), c.pitch, {1, 0, 0});
    }

    return glm::inverse(Maths::Transform(tr.position, tr.orientation));   
}

glm::mat4 MakeProj(const ecs::Entity& entity)
{
    float fov = glm::radians(70.0f);

    if (entity.Has<Camera3DComponent>()) {
        fov = glm::radians(entity.Get<Camera3DComponent>().fov);
    }

    return glm::perspective(fov, Viewport::CurrentAspectRatio(), 0.01f, 1000.0f);
}

}