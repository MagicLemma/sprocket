#include "CameraSystem.h"
#include "Components.h"
#include "WindowEvent.h"
#include "Scene.h"
#include "Entity.h"

namespace Sprocket {

CameraSystem::CameraSystem(float aspectRatio)
    : d_aspectRatio(aspectRatio)
{}

void CameraSystem::OnStartup(Scene& scene)
{
    scene.OnAdd<CameraComponent>([&](Entity& entity) {
        auto& camera = entity.Get<CameraComponent>();
        camera.projection = Maths::Perspective(
            d_aspectRatio, camera.fov, 0.1f, 1000.0f
        );
    });
}

void CameraSystem::OnEvent(Scene& scene, Event& event)
{
    if (auto e = event.As<WindowResizeEvent>()) {
        d_aspectRatio = e->AspectRatio();

        scene.Each<CameraComponent>([&](Entity& entity) {
            auto& camera = entity.Get<CameraComponent>();
            camera.projection = Maths::Perspective(
                d_aspectRatio, camera.fov, 0.1f, 1000.0f
            );
        });
    }
}

}