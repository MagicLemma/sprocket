#include "basic_systems.h"
#include "Components.h"

#include <glm/gtx/norm.hpp>

namespace spkt {

void animation_system(spkt::registry& registry, double dt)
{
    for (auto id : registry.view<MeshAnimationComponent>()) {
        auto& ac = registry.get<MeshAnimationComponent>(id);
        ac.time += (float)dt * ac.speed;
    }
}

void camera_system(spkt::registry& registry, double dt)
{
    auto singleton = registry.find<Singleton>();
    if (!registry.valid(singleton)) {
        return;
    }

    const auto& input = registry.get<InputSingleton>(singleton);
    float aspect_ratio = input.window_width / input.window_height;

    for (auto entity : registry.view<Camera3DComponent>()) {
        auto& camera = registry.get<Camera3DComponent>(entity);
        camera.projection = glm::perspective(
            camera.fov, aspect_ratio, 0.1f, 1000.0f
        );
    }
}

void path_follower_system(spkt::registry& registry, double dt)
{
    for (auto entity : registry.view<PathComponent>()) {
        auto& transform = registry.get<Transform3DComponent>(entity);
        auto& path = registry.get<PathComponent>(entity);
        if (path.markers.empty()) { return; }
        
        glm::vec3 direction = glm::normalize(path.markers.front() - transform.position);
        glm::vec3 advance = path.speed * (float)dt * direction;

        if (glm::length2(advance) < glm::length2(path.markers.front() - transform.position)) {
            transform.position += advance;
        }
        else {
            transform.position = path.markers.front();
            path.markers.pop();
        }
    }
}

}