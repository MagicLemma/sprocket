#include "PathFollower.h"
#include "Components.h"
#include "Maths.h"
#include "ECS.h"

namespace Sprocket {

void PathFollower::on_update(ecs::Registry& registry, const ev::Dispatcher&, double dt)
{
    for (auto entity : registry.view<PathComponent>()) {
        auto& transform = entity.get<Transform3DComponent>();
        auto& path = entity.get<PathComponent>();
        if (path.markers.empty()) { return; }
        
        glm::vec3 direction = glm::normalize(path.markers.front() - transform.position);
        glm::vec3 advance = path.speed * (float)dt * direction;

        auto MagSq = [](const glm::vec3& v) {
            return v.x*v.x + v.y*v.y + v.z*v.z;
        };

        if (MagSq(advance) < MagSq(path.markers.front() - transform.position)) {
            transform.position += advance;
        } else {
            transform.position = path.markers.front();
            path.markers.pop();
        }
    }
}

}