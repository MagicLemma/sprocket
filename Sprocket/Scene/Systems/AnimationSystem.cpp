#include "AnimationSystem.h"
#include "Components.h"
#include "ECS.h"
#include "Scene.h"

namespace Sprocket {

void AnimationSystem::on_update(ecs::Registry& registry, const ev::Dispatcher&, double dt)
{
    for (auto entity : registry.view<MeshAnimationComponent>()) {
        auto& ac = entity.get<MeshAnimationComponent>();
        ac.time += (float)dt * ac.speed;
    }
}

}