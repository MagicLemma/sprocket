#include "PathFollower.h"

void PathFollower::UpdateEntity(double dt, Sprocket::Entity& entity)
{
    using namespace Sprocket;
    if (!entity.Has<PathComponent>()) { return; }

    auto& path = entity.Get<PathComponent>();
    if (path.markers.empty()) { return; }
    
    Maths::vec3 direction = path.markers.front() - entity.Position();
    Maths::Normalise(direction);
    Maths::vec3 advance = path.speed * (float)dt * direction;

    auto MagSq = [](const Maths::vec3& v) {
        return v.x*v.x + v.y*v.y + v.z*v.z;
    };

    if (MagSq(advance) < MagSq(path.markers.front() - entity.Position())) {
        entity.Position() += advance;
    } else {
        entity.Position() = path.markers.front();
        path.markers.pop();
    }
}