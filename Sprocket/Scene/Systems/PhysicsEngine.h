#pragma once
#include "Maths.h"
#include "ECS.h"
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

    float       d_lastFrameLength;
    
    bool        d_running = false;

    bool IsOnFloor(ECS::Entity entity) const; 
        // Returns true if the given Entity is colliding with another
        // Entity directly below it. TODO: Make this more general.

public:
    PhysicsEngine(const glm::vec3& gravity);
    ~PhysicsEngine() {}

    void OnStartup(Scene& scene) override;
    void OnUpdate(Scene& scene, double dt) override;

    void Running(bool isRunning);
    bool Running() const { return d_running; }

    ECS::Entity Raycast(const glm::vec3& base, const glm::vec3& direction);
        // Given a position in the world and a direction from that point,
        // return a pointer to the entity that it hits, or null if it
        // does not.
};

}