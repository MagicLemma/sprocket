#include "PathFollower.h"

void PathFollower::OnUpdate(std::map<entt::entity, Sprocket::Entity>& entities, double dt)
{
    using namespace Sprocket;

    for (auto& [id, entity] : entities) {
        if (!entity.Has<TransformComponent>()) { continue; }
        if (!entity.Has<PathComponent>()) { continue; }

        auto& tr = entity.Get<TransformComponent>();
        auto& path = entity.Get<PathComponent>();
        if (path.markers.empty()) { continue; }
        
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
}