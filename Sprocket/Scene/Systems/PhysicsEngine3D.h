#pragma once
#include "ECS.h"

namespace spkt {

struct physics_runtime;
struct rigid_body_runtime;
struct collider_runtime;

static constexpr float PHYSICS_TIME_STEP = 1.0f / 60.0f;

void physics_system(spkt::registry& registry, double dt);

}