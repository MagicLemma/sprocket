#pragma once

namespace Sprocket {

class Scene;
namespace ev { class Event; }
namespace ecs { class Registry; }

class EntitySystem
{
public:
    EntitySystem() = default;
    virtual ~EntitySystem() {};

    virtual void on_startup(ecs::Registry& registry) {};
        // Called once when starting the scene. There may be
        // entities in the scene by this point.
    
    virtual void on_update(ecs::Registry& registry, double dt) {};
        // Called every tick of the game loop.

    virtual void on_event(ecs::Registry& registry, ev::Event& event) {};
        // Called with every event so systems can consume them.

private:
    EntitySystem(const EntitySystem&) = delete;
    EntitySystem& operator=(const EntitySystem&) = delete;
};

}