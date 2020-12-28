#include "CameraSystem.h"
#include "Components.h"
#include "WindowEvent.h"
#include "Scene.h"
#include "ECS.h"

namespace Sprocket {

CameraSystem::CameraSystem(float aspectRatio)
    : d_aspectRatio(aspectRatio)
{}

void CameraSystem::OnStartup(Scene& scene)
{
    scene.Reg()->OnAdd<CameraComponent>([&](ECS::Entity entity) {
        auto& camera = entity.Get<CameraComponent>();
        camera.projection = glm::perspective(
            camera.fov, d_aspectRatio, 0.1f, 1000.0f
        );
    });
}

void CameraSystem::OnEvent(Scene& scene, Event& event)
{
    if (auto e = event.As<WindowResizeEvent>()) {
        d_aspectRatio = e->AspectRatio();

        for (auto entity : scene.Reg()->View<CameraComponent>()) {
            auto& camera = entity.Get<CameraComponent>();
            camera.projection = glm::perspective(
                camera.fov, d_aspectRatio, 0.1f, 1000.0f
            );
        }
    }
}

}