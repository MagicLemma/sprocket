#pragma once
#include "ECS.h"
#include "ParticleManager.h"

namespace spkt {

// TODO: Integrate particles better with the ECS.
void particle_system_init(spkt::registry& registry, ParticleManager* particle_manager);
void particle_system(spkt::registry& registry, double dt);

}