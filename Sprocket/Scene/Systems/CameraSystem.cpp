#include "CameraSystem.h"
#include "Components.h"
#include "Events.h"
#include "Scene.h"
#include "ECS.h"

namespace Sprocket {

CameraSystem::CameraSystem(float aspectRatio)
    : d_aspectRatio(aspectRatio)
{}

void CameraSystem::OnStartup(Scene& scene)
{
    scene.Entities().OnAdd<Camera3DComponent>([&](ecs::Entity entity) {
        auto& camera = entity.Get<Camera3DComponent>();
        camera.projection = glm::perspective(
            camera.fov, d_aspectRatio, 0.1f, 1000.0f
        );
    });
}

void CameraSystem::OnEvent(Scene& scene, ev::Event& event)
{
    if (auto e = event.get_if<ev::WindowResize>()) {
        d_aspectRatio = (float)e->width / e->height;

        for (auto entity : scene.Entities().View<Camera3DComponent>()) {
            auto& camera = entity.Get<Camera3DComponent>();
            camera.projection = glm::perspective(
                camera.fov, d_aspectRatio, 0.1f, 1000.0f
            );
        }
    }
}

}