#include "AnimationSystem.h"
#include "Components.h"
#include "Entity.h"
#include "Scene.h"

namespace Sprocket {

void AnimationSystem::OnUpdate(Scene& scene, double dt, bool active)
{
    if (!active) { return; }
    
    scene.Each<AnimationComponent>([dt](Entity& entity) {
        auto& ac = entity.Get<AnimationComponent>();
        ac.time += (float)dt * ac.speed;
    });
}

}