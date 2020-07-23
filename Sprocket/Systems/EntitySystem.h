#pragma once
#include "Entity.h"
#include "Event.h"

#include <map>

namespace Sprocket {

class EntityManager;

class EntitySystem
{
public:
    EntitySystem() = default;

    virtual void OnStartup(EntityManager& manager) {};
        // Called once when the EntityManager starts up.
    
    virtual void OnUpdate(EntityManager& manager, double dt) {};
        // Called every tick of the game loop.

    virtual void RegisterEntity(const Entity& entity) {};
    virtual void DeregisterEntity(const Entity& entity) {};
        // Register and deregister entities with this system.

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