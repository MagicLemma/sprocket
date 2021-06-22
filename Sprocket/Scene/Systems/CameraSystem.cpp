#include "CameraSystem.h"
#include "Components.h"
#include "Events.h"
#include "Scene.h"
#include "ECS.h"

namespace Sprocket {

CameraSystem::CameraSystem(float aspectRatio)
    : d_aspectRatio(aspectRatio)
{}

void CameraSystem::on_event(spkt::registry& registry, ev::Event& event)
{
    if (auto e = event.get_if<spkt::added<Camera3DComponent>>()) {
        spkt::entity entity = e->entity;
        auto& camera = entity.get<Camera3DComponent>();
        camera.projection = glm::perspective(
            camera.fov, d_aspectRatio, 0.1f, 1000.0f
        );
    }
    else if (auto e = event.get_if<ev::WindowResize>()) {
        d_aspectRatio = (float)e->width / e->height;
        for (auto entity : registry.view<Camera3DComponent>()) {
            auto& camera = registry.get<Camera3DComponent>(entity);
            camera.projection = glm::perspective(
                camera.fov, d_aspectRatio, 0.1f, 1000.0f
            );
        }
    }
}

}