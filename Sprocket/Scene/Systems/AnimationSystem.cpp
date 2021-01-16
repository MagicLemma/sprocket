#include "AnimationSystem.h"
#include "Components.h"
#include "ECS.h"
#include "Scene.h"

namespace Sprocket {

void AnimationSystem::OnUpdate(Scene& scene, double dt)
{
    for (auto entity : scene.Entities().View<MeshAnimationComponent>()) {
        auto& ac = entity.Get<MeshAnimationComponent>();
        ac.time += (float)dt * ac.speed;
    }
}

}