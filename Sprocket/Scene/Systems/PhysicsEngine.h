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

    void AddCollider(Entity entity);
        // Registers the collider on the entity with the
        // physics engine.

public:
    PhysicsEngine(const Maths::vec3& gravity);
    ~PhysicsEngine() {}

    void OnStartup(Scene& scene) override;

    void OnUpdate(Scene& scene, double dt) override;

    void Running(bool isRunning);
    bool Running() const { return d_running; }

    void SetSpeedFactor(SpeedFactor sf) { d_speedFactor = sf; }

    Entity Raycast(const Maths::vec3& base, const Maths::vec3& direction);
        // Given a position in the world and a direction from that point,
        // return a pointer to the entity that it hits, or null if it
        // does not.

    // Entity Manipulations
    void ApplyForce(Entity entity, const Maths::vec3& force);
        // Apply the given force to the centre of mass of the given Entity.
        // If the Entity has no PhysicsComponent, this is a noop. The
        // force should not be divided by dt as the physics engine will do
        // this itself.

    void MakeUpright(Entity entity, float yaw = 0);
        // Sets the orientation of the given Entity to make it upright. The yaw
        // is the angle in degrees anti-clockwise from the negative Z axis.

    bool IsOnFloor(Entity entity) const; 
        // Returns true if the given Entity is colliding with another
        // Entity directly below it. TODO: Make this more general.

    void RefreshTransform(Entity entity);
};

}