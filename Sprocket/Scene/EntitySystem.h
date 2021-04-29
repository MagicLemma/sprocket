#pragma once
#include "ECS.h"

namespace Sprocket {
namespace ev { class Event; class Dispatcher; }

class EntitySystem
{
public:
    EntitySystem() = default;
    virtual ~EntitySystem() {};

    virtual void on_startup(spkt::registry& registry, ev::Dispatcher& dispatcher) {};
        // Called once when starting the scene. There may be
        // entities in the scene by this point.
    
    virtual void on_update(spkt::registry& registry, const ev::Dispatcher& dispatcher, double dt) {};
        // Called every tick of the game loop.

private:
    EntitySystem(const EntitySystem&) = delete;
    EntitySystem& operator=(const EntitySystem&) = delete;
};

}