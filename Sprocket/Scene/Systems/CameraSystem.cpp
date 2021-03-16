#include "CameraSystem.h"
#include "Components.h"
#include "Events.h"
#include "Scene.h"
#include "ECS.h"

namespace Sprocket {

CameraSystem::CameraSystem(float aspectRatio)
    : d_aspectRatio(aspectRatio)
{}

void CameraSystem::on_startup(ecs::Registry& registry, ev::Dispatcher& dispatcher)
{
    dispatcher.subscribe<ecs::ComponentAddedEvent<Camera3DComponent>>([&](ev::Event& event) {
        auto data = event.get<ecs::ComponentAddedEvent<Camera3DComponent>>();

        auto& camera = data.entity.get<Camera3DComponent>();
        camera.projection = glm::perspective(
            camera.fov, d_aspectRatio, 0.1f, 1000.0f
        );
    });

    dispatcher.subscribe<ev::WindowResize>([&](ev::Event& event) {
        auto data = event.get<ev::WindowResize>();

        d_aspectRatio = (float)data.width / data.height;
        for (auto entity : registry.view<Camera3DComponent>()) {
            auto& camera = entity.get<Camera3DComponent>();
            camera.projection = glm::perspective(
                camera.fov, d_aspectRatio, 0.1f, 1000.0f
            );
        }
    });
}

}