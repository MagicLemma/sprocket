#pragma once
#include "Events.h"
#include "ECS.h"
#include "EntitySystem.h"

namespace spkt {

struct GameGrid : public EntitySystem
{
    void on_startup(spkt::registry& registry) override;
    void on_update(spkt::registry& registry, double dt) override;
};

}