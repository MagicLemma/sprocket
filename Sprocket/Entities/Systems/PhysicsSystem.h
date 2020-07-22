#pragma once
#include "NewSystem.h"

#include <memory>

namespace Sprocket {

struct PhysicsSystemImpl;

class PhysicsSystem : public NewSystem
{
    std::shared_ptr<PhysicsSystemImpl> d_impl;

    const float d_timeStep;

    float       d_lastFrameLength;
    
    bool        d_running = false;

    void UpdatePlayer(double dt, entt::entity entity);
        // If this Entity has a PlayerComponent, perform the
        // necessary updates.

    void AddCollider(entt::entity entity);
        // Registers the collider on the entity with the
        // physics engine.
    
public:
    void OnAttach(entt::registry& registry) override;
    void OnUpdate(double dt, entt::registry& registry) override;
    void OnEvent(const Event& event, entt::registry& registry) override;
};

}