#pragma once
#include "ECS.h"
#include "EntitySystem.h"

#include <memory>

#include <glm/glm.hpp>

namespace spkt {

struct physics_runtime;
struct rigid_body_runtime;
struct collider_runtime;

static constexpr float PHYSICS_TIME_STEP = 1.0f / 60.0f;

void physics_system_init(spkt::registry& registry);
void physics_system(spkt::registry& registry, double dt);

}