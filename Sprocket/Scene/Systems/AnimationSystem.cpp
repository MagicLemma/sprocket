#include "AnimationSystem.h"
#include "Components.h"
#include "ECS.h"
#include "Scene.h"

namespace Sprocket {

void AnimationSystem::on_update(spkt::registry& registry, double dt)
{
    for (auto id : registry.view<MeshAnimationComponent>()) {
        auto& ac = registry.get<MeshAnimationComponent>(id);
        ac.time += (float)dt * ac.speed;
    }
}

}