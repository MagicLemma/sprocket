#pragma once
#include "ECS.h"
#include "EntitySystem.h"

#include <memory>

#include <glm/glm.hpp>

namespace Sprocket {

struct rigid_body_runtime;
struct collider_runtime;

struct PhysicsEngine3DImpl;

class PhysicsEngine3D : public EntitySystem
{
    static constexpr float TIME_STEP = 1.0f / 120.0f;

    std::unique_ptr<PhysicsEngine3DImpl> d_impl;

public:
    PhysicsEngine3D(const glm::vec3& gravity = {0.0f, -9.81f, 0.0f});
    ~PhysicsEngine3D() = default;

    void on_startup(spkt::registry& registry) override;
    void on_update(spkt::registry& registry, double dt) override;
};

}