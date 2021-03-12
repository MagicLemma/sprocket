#include "CameraSystem.h"
#include "Components.h"
#include "Events.h"
#include "Scene.h"
#include "ECS.h"

namespace Sprocket {

CameraSystem::CameraSystem(float aspectRatio)
    : d_aspectRatio(aspectRatio)
{}

void CameraSystem::OnEvent(Scene& scene, ev::Event& event)
{
    if (auto data = event.get_if<ecs::ComponentAddedEvent<Camera3DComponent>>()) {
        auto& camera = data->entity.Get<Camera3DComponent>();
        camera.projection = glm::perspective(
            camera.fov, d_aspectRatio, 0.1f, 1000.0f
        );
    }
    else if (auto data = event.get_if<ev::WindowResize>()) {
        d_aspectRatio = (float)data->width / data->height;

        for (auto entity : scene.Entities().View<Camera3DComponent>()) {
            auto& camera = entity.Get<Camera3DComponent>();
            camera.projection = glm::perspective(
                camera.fov, d_aspectRatio, 0.1f, 1000.0f
            );
        }
    }
}

}