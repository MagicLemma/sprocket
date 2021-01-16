#include "Camera.h"
#include "Log.h"
#include "Components.h"

namespace Sprocket {

glm::mat4 MakeView(const ecs::Entity& entity)
{
    if (!entity.Has<Transform3DComponent>()) {
        SPKT_LOG_ERROR("Camera has no transform component!");
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
    if (!entity.Has<Camera3DComponent>()) {
        return glm::mat4(1.0);
    }

    return entity.Get<Camera3DComponent>().projection;
}

}