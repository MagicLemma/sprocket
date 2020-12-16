#include "AnimationSystem.h"
#include "Components.h"
#include "ECS.h"
#include "Scene.h"

namespace Sprocket {

void AnimationSystem::OnUpdate(Scene& scene, double dt)
{
    scene.Each<AnimationComponent>([dt](ECS::Entity& entity) {
        auto& ac = entity.Get<AnimationComponent>();
        ac.time += (float)dt * ac.speed;
    });
}

}