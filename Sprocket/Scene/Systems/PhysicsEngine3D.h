#pragma once
#include "ECS.h"
#include "EntitySystem.h"

#include <memory>

#include <glm/glm.hpp>

namespace Sprocket {

struct PhysicsEngine3DImpl;

class PhysicsEngine3D : public EntitySystem
{
    static constexpr float TIME_STEP = 1.0f / 60.0f;

    std::unique_ptr<PhysicsEngine3DImpl> d_impl;

    bool IsOnFloor(ecs::Entity entity) const; 
        // Returns true if the given Entity is colliding with another
        // Entity directly below it. TODO: Make this more general.

public:
    PhysicsEngine3D(const glm::vec3& gravity = {0.0f, -9.81f, 0.0f});
    ~PhysicsEngine3D() = default;

    void OnStartup(Scene& scene) override;
    void OnUpdate(Scene& scene, double dt) override;

    ecs::Entity Raycast(const glm::vec3& base, const glm::vec3& direction);
        // Given a position in the world and a direction from that point,
        // return a pointer to the entity that it hits, or null if it
        // does not.
};

}