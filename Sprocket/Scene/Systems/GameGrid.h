#pragma once
#include "apecs.hpp"

namespace spkt {

void game_grid_system_init(apx::registry& registry);
void game_grid_system(apx::registry& registry, double dt);

}