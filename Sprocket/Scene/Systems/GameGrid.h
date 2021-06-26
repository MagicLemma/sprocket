#pragma once
#include "Events.h"
#include "ECS.h"
#include "EntitySystem.h"

namespace Sprocket {

struct GameGrid : public EntitySystem
{
    void on_startup(spkt::registry& registry) override;
    void on_event(spkt::registry& registry, ev::Event& event) override;
    void on_update(spkt::registry& registry, double dt) override;
};

}