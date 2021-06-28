#include "CameraSystem.h"
#include "Components.h"
#include "Events.h"
#include "Scene.h"
#include "ECS.h"

#include <cassert>

namespace spkt {
namespace {

float aspect_ratio(const InputSingleton& input)
{
    assert(input.window_height > 0);
    return input.window_width / input.window_height;
}

}

void CameraSystem::on_update(spkt::registry& registry, double dt)
{
    auto singleton = registry.find<Singleton>();
    if (registry.valid(singleton)) {
        const auto& input = registry.get<InputSingleton>(singleton);
        for (auto entity : registry.view<Camera3DComponent>()) {
            auto& camera = registry.get<Camera3DComponent>(entity);
            camera.projection = glm::perspective(
                camera.fov, aspect_ratio(input), 0.1f, 1000.0f
            );
        }
    }
}

}