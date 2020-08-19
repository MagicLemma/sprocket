#include "CameraSystem.h"
#include "Components.h"
#include "WindowEvent.h"
#include "Scene.h"
#include "Entity.h"

namespace Sprocket {

void CameraSystem::OnUpdate(Scene& scene, double dt)
{
    scene.Each<CameraComponent>([&](Entity& entity) {
        auto& camera = entity.Get<CameraComponent>();
        camera.projection = Maths::Perspective(
            d_aspectRatio,
            camera.fov,
            0.1f,
            1000.0f
        );
    });
}

void CameraSystem::OnEvent(Event& event)
{
    if (auto e = event.As<WindowResizeEvent>()) {
        d_aspectRatio = e->AspectRatio();
    }
}

}