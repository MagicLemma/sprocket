#pragma once
#include "ECS.h"
#include "EntitySystem.h"

#include <memory>

#include <glm/glm.hpp>

namespace Sprocket {

struct physics_runtime;
struct rigid_body_runtime;
struct collider_runtime;

class PhysicsEngine3D : public EntitySystem
{
    static constexpr float TIME_STEP = 1.0f / 60.0f;

    void on_startup(spkt::registry& registry) override;
    void on_update(spkt::registry& registry, double dt) override;
};

}