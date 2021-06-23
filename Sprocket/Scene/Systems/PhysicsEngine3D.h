#pragma once
#include "ECS.h"
#include "EntitySystem.h"

#include <memory>

#include <glm/glm.hpp>

namespace Sprocket {

struct PhysicsEngine3DImpl;

struct CollisionEvent
{
    spkt::entity entity1;
    spkt::entity entity2;
    CollisionEvent(spkt::entity e1, spkt::entity e2) : entity1(e1), entity2(e2) {}
};

class PhysicsEngine3D : public EntitySystem
{
    static constexpr float TIME_STEP = 1.0f / 120.0f;

    std::unique_ptr<PhysicsEngine3DImpl> d_impl;

public:
    PhysicsEngine3D(const glm::vec3& gravity = {0.0f, -9.81f, 0.0f});
    ~PhysicsEngine3D() = default;

    void on_startup(spkt::registry& registry) override;
    void on_event(spkt::registry& registry, ev::Event& event) override;
    void on_update(spkt::registry& registry, double dt) override;

    spkt::entity Raycast(const glm::vec3& base, const glm::vec3& direction);
        // Given a position in the world and a direction from that point,
        // return a pointer to the entity that it hits, or null if it
        // does not.
};

}