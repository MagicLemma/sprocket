#pragma once
#include "Entity.h"
#include "Event.h"

namespace Sprocket {

class EntitySystem
{
public:
    EntitySystem() = default;
    
    virtual void UpdateEntity(double dt, Entity& entity) {};
        // Called once per entity per frame and before the system updates.

    virtual void PostUpdateEntity(double dt, Entity& entity) {};
        // Called once per entity per frame and before the system updates.

    virtual void UpdateSystem(double dt) {};
        // Called once per frame, this should update any system speific data.

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