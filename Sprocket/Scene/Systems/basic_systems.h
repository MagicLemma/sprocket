#pragma once
#include "ECS.h"

namespace spkt {

void animation_system(spkt::registry& registry, double dt);
void camera_system(spkt::registry& registry, double dt);
void path_follower_system(spkt::registry& registry, double dt);

}