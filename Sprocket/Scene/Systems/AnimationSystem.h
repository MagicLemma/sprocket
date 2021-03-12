#pragma once
#include "EntitySystem.h"

namespace Sprocket {

class AnimationSystem : public EntitySystem
{
public:
    void OnUpdate(ecs::Registry& registry, double dt) override;
};

}