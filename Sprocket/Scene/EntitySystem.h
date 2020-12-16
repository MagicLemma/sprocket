#pragma once
#include "ECS.h"
#include "Event.h"

#include <map>

namespace Sprocket {

class Scene;

class EntitySystem
{
public:
    EntitySystem() = default;

    virtual void OnStartup(Scene& scene) {};
        // Called once when starting the scene. There may be
        // entities in the scene by this point.
    
    virtual void OnUpdate(Scene& scene, double dt) {};
        // Called every tick of the game loop.

    virtual void OnEvent(Scene& scene, Event& event) {};
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