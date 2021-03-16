#pragma once

namespace Sprocket {

class Scene;
namespace ev { class Event; class Dispatcher; }
namespace ecs { class Registry; }

class EntitySystem
{
public:
    EntitySystem() = default;
    virtual ~EntitySystem() {};

    virtual void on_startup(ecs::Registry& registry, ev::Dispatcher& dispatcher) {};
        // Called once when starting the scene. There may be
        // entities in the scene by this point.
    
    virtual void on_update(ecs::Registry& registry, const ev::Dispatcher& dispatcher, double dt) {};
        // Called every tick of the game loop.

private:
    EntitySystem(const EntitySystem&) = delete;
    EntitySystem& operator=(const EntitySystem&) = delete;
};

}