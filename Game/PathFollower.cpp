#include "PathFollower.h"

void PathFollower::UpdateEntity(double dt, Sprocket::Entity& entity)
{
    using namespace Sprocket;
    if (!entity.Has<TransformComponent>()) { return; }
    if (!entity.Has<PathComponent>()) { return; }

    auto& tr = entity.Get<TransformComponent>();
    auto& path = entity.Get<PathComponent>();
    if (path.markers.empty()) { return; }
    
    Maths::vec3 direction = path.markers.front() - tr.position;
    Maths::Normalise(direction);
    Maths::vec3 advance = path.speed * (float)dt * direction;

    auto MagSq = [](const Maths::vec3& v) {
        return v.x*v.x + v.y*v.y + v.z*v.z;
    };

    if (MagSq(advance) < MagSq(path.markers.front() - tr.position)) {
        tr.position += advance;
    } else {
        tr.position = path.markers.front();
        path.markers.pop();
    }
}