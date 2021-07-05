#pragma once
#include "apecs.hpp"
#include "ParticleManager.h"

namespace spkt {

// TODO: Integrate particles better with the ECS.
void particle_system_init(apx::registry& registry);
void particle_system(apx::registry& registry, double dt);

}