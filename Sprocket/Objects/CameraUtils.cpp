#include "CameraUtils.h"

namespace Sprocket {
namespace CameraUtils {

Maths::mat4 MakeView(const Entity& entity)
{
    if (!entity.Has<TransformComponent>()) {
        SPKT_LOG_ERROR("Camera has no transform component!");
        return Maths::mat4{1.0};
    }

    auto tr = entity.Get<TransformComponent>();

    if (entity.Has<CameraComponent>()) {
        const auto& c = entity.Get<CameraComponent>();
        tr.orientation *= Maths::Rotate({1, 0, 0}, c.pitch);
    }

    return Maths::Inverse(tr.Transform());   
}

Maths::mat4 MakeProj(const Entity& entity)
{
    if (!entity.Has<CameraComponent>()) {
        return Maths::mat4(1.0);
    }

    return entity.Get<CameraComponent>().projection;
}

}
}