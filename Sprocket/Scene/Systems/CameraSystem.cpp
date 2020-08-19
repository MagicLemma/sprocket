#include "CameraSystem.h"
#include "Components.h"
#include "WindowEvent.h"
#include "Scene.h"
#include "Entity.h"

namespace Sprocket {

void CameraSystem::OnEvent(Scene& scene, Event& event)
{
    if (auto e = event.As<WindowResizeEvent>()) {
        float aspectRatio = e->AspectRatio();

        scene.Each<CameraComponent>([&](Entity& entity) {

        });
    }
}

}