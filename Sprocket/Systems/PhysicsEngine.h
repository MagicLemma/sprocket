#pragma once
#include "Maths.h"
#include "Entity.h"
#include "EntitySystem.h"

#include <unordered_map>
#include <utility>
#include <memory>
#include <variant>

namespace Sprocket {

enum class SpeedFactor
{
    QUARTER,
    HALF,
    NORMAL,
    DOUBLE,
    QUADRUPLE
};

struct PhysicsEngineImpl;

class PhysicsEngine : public EntitySystem
{
    std::shared_ptr<PhysicsEngineImpl> d_impl;

    const float d_timeStep;
    SpeedFactor d_speedFactor;

    float       d_lastFrameLength;
    
    bool        d_running = false;

    void updatePlayer(float dt, Entity& entity);
        // If this Entity has a PlayerComponent, perform the
        // necessary updates.

    void addCollider(const Entity& entity);
        // Registers the collider on the entity with the
        // physics engine.

public:
    PhysicsEngine(const Maths::vec3& gravity);
    ~PhysicsEngine() {}

    void updateEntity(float dt, Entity& entity) override;
    void updateSystem(float dt) override;

    void registerEntity(const Entity& entity) override;
    void deregisterEntity(const Entity& entity) override;

    void onComponentAttach(const Entity& entity, const Component& component) override;
    void onComponentDetach(const Entity& entity, const Component& component) override; 

    void running(bool isRunning);
    bool running() const { return d_running; }

    void setSpeedFactor(SpeedFactor sf) { d_speedFactor = sf; }

    Entity* raycast(const Maths::vec3& base, const Maths::vec3& direction);
        // Given a position in the world and a direction from that point,
        // return a pointer to the entity that it hits, or nullptr if it
        // does not.

    // Entity Manipulations
    void applyForce(Entity* entity, const Maths::vec3& force);
        // Apply the given force to the centre of mass of the given Entity.
        // If the Entity has no PhysicsComponent, this is a noop.

    void makeUpright(Entity* entity, float yaw = 0);
        // Sets the orientation of the given Entity to make it upright. The yaw
        // is the angle in degrees anti-clockwise from the negative Z axis.

    bool isOnFloor(const Entity* entity) const; 
        // Returns true if the given Entity is colliding with another
        // Entity directly below it. TODO: Make this more general.

    void refreshTransform(const Entity* entity);
};

}