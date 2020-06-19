#include "CameraUtils.h"

namespace Sprocket {
namespace CameraUtils {

Maths::mat4 MakeView(const Entity& entity)
{
    Maths::vec3 position = entity.Position();
    Maths::quat orientation = entity.Orientation();

    if (entity.Has<CameraComponent>()) {
        const auto& c = entity.Get<CameraComponent>();
        orientation *= Maths::Rotate({1, 0, 0}, c.pitch);
    }

    return Maths::Inverse(Maths::Transform(position, orientation));   
}

Maths::mat4 MakeProj(const Entity& entity)
{
    if (!entity.Has<CameraComponent>()) {
        return Maths::mat4(1.0);
    }
    return entity.Get<CameraComponent>().lens->Projection();
}

}
}