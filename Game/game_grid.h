#pragma once
#include <Game/ecs/ecs.h>

void game_grid_system_init(game::registry& registry);
void game_grid_system(game::registry& registry, double dt);