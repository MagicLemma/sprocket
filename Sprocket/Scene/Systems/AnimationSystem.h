#pragma once
#include "EntitySystem.h"

namespace Sprocket {

class AnimationSystem : public EntitySystem
{
public:
    void on_update(ecs::Registry& registry, const ev::Dispatcher& dispatcher, double dt) override;
};

}