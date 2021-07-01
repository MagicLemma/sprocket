#pragma once
#include "ECS.h"

namespace spkt {

void game_grid_system_init(spkt::registry& registry);
void game_grid_system(spkt::registry& registry, double dt);

}