#pragma once
#include "Maths.h"
#include "Entity.h"
#include "EntitySystem.h"

#include <unordered_map>
#include <utility>
#include <memory>
#include <variant>

namespace Sprocket {

struct PhysicsEngineImpl;

class PhysicsEngine : public EntitySystem
{
    std::shared_ptr<PhysicsEngineImpl> d_impl;

    const float d_timeStep;
    bool        d_running = false;

    void updatePlayer(Entity& entity);
        // If this Entity has a PlayerComponent, perform the
        // necessary updates.

public:
    PhysicsEngine(const Maths::vec3& gravity);
    ~PhysicsEngine() {}

    void updateSystem(float dt) override;
    void preUpdateEntity(Entity& entity) override;
    void postUpdateEntity(Entity& entity) override;

    void registerEntity(const Entity& entity) override;
    void deregisterEntity(const Entity& entity) override;

    void running(bool isRunning);
    bool running() const { return d_running; }

    Entity* raycast(const Maths::vec3& base, const Maths::vec3& direction);
        // Given a position in the world and a direction from that point,
        // return a pointer to the entity that it hits, or nullptr if it
        // does not.
};

}