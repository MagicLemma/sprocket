#pragma once
#include "apecs.hpp"

namespace spkt {

void particle_system_init(apx::registry& registry);
void particle_system(apx::registry& registry, double dt);

}