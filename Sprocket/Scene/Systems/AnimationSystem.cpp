#include "AnimationSystem.h"
#include "Components.h"
#include "ECS.h"
#include "Scene.h"

namespace Sprocket {

void AnimationSystem::OnUpdate(ecs::Registry& registry, double dt)
{
    for (auto entity : registry.View<MeshAnimationComponent>()) {
        auto& ac = entity.Get<MeshAnimationComponent>();
        ac.time += (float)dt * ac.speed;
    }
}

}