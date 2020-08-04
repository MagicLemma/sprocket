#pragma once
#include "Entity.h"
#include "Event.h"

namespace Sprocket {

class EntityManager;

class EntitySystem
{
public:
    EntitySystem() = default;

    virtual void OnStartup(EntityManager& manager) {};
        // Called once when the EntityManager starts up.
    
    virtual void OnUpdate(EntityManager& manager, double dt, bool active) {};
        // Called every tick of the game loop.

    virtual void OnEvent(Event& event) {};
        // Called with every event so systems can consume them.

private:
    // EntitySystems can not be copied.
    EntitySystem(const EntitySystem&) = delete;
    EntitySystem& operator=(const EntitySystem&) = delete;

    // EntitySystems can not be moved.
    EntitySystem(EntitySystem&&) = delete;
    EntitySystem& operator=(EntitySystem&&) = delete;
};

}