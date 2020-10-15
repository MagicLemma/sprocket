#include "AnimationSystem.h"
#include "Components.h"
#include "Entity.h"
#include "Scene.h"

namespace Sprocket {

void AnimationSystem::OnUpdate(Scene& scene, double dt)
{
    scene.Each<AnimationComponent>([dt](Entity& entity) {
        entity.Get<AnimationComponent>().time += (float)dt;
    });
}

}