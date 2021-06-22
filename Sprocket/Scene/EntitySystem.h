#pragma once
#include "ECS.h"

namespace Sprocket {
namespace ev { class Event; }

class EntitySystem
{
public:
    EntitySystem() = default;
    virtual ~EntitySystem() {};

    virtual void on_startup(spkt::registry& registry) {};
        // Called once when starting the scene. There may be
        // entities in the scene by this point.

    virtual void on_event(spkt::registry& registry, ev::Event& event) {};
    
    virtual void on_update(spkt::registry& registry, double dt) {};
        // Called every tick of the game loop.

private:
    EntitySystem(const EntitySystem&) = delete;
    EntitySystem& operator=(const EntitySystem&) = delete;
};

}