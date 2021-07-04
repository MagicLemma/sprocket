#pragma once
#include "apecs.hpp"

namespace spkt {
namespace ev { class Event; }

class EntitySystem
{
public:
    EntitySystem() = default;
    virtual ~EntitySystem() {};

    virtual void on_startup(apx::registry& registry) {};
        // Called once when starting the scene. There may be
        // entities in the scene by this point.
    
    virtual void on_update(apx::registry& registry, double dt) {};
        // Called every tick of the game loop.

private:
    EntitySystem(const EntitySystem&) = delete;
    EntitySystem& operator=(const EntitySystem&) = delete;
};

}