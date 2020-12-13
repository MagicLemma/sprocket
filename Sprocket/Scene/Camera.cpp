#include "Camera.h"
#include "Log.h"
#include "Components.h"

namespace Sprocket {

glm::mat4 MakeView(const ECS::Entity& entity)
{
    if (!entity.Has<TransformComponent>()) {
        SPKT_LOG_ERROR("Camera has no transform component!");
        return glm::mat4{1.0};
    }

    auto tr = entity.Get<TransformComponent>();

    if (entity.Has<CameraComponent>()) {
        const auto& c = entity.Get<CameraComponent>();
        tr.orientation *= glm::rotate(glm::identity<glm::quat>(), c.pitch, {1, 0, 0});
    }

    return glm::inverse(Maths::Transform(tr.position, tr.orientation));   
}

glm::mat4 MakeProj(const ECS::Entity& entity)
{
    if (!entity.Has<CameraComponent>()) {
        return glm::mat4(1.0);
    }

    return entity.Get<CameraComponent>().projection;
}

}