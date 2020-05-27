#pragma once
#include "Entity.h"
#include "Event.h"

namespace Sprocket {

class EntitySystem
{
public:
    EntitySystem() = default;
    
    virtual void updateEntity(float dt, Entity& entity) {};
        // Called once per entity per frame and before the system updates.

    virtual void updateSystem(float dt) = 0;
        // Called once per frame, this should update any system speific data.

    virtual void registerEntity(const Entity& entity) {};
    virtual void deregisterEntity(const Entity& entity) {};
        // Register and deregister entities with this system.

    virtual void onComponentAttach(const Entity& entity,
                                   const Component& component) {};
    virtual void onComponentDetach(const Entity& entity,
                                   const Component& component) {};
        // Called whenever a component is added or removed from an entity.

    virtual void handleEvent(Event& event) {};
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