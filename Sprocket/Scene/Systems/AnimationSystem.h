#pragma once
#include "EntitySystem.h"
#include "ECS.h"

namespace Sprocket {

class AnimationSystem : public EntitySystem
{
public:
    void on_update(spkt::registry& registry, const ev::Dispatcher& dispatcher, double dt) override;
};

}