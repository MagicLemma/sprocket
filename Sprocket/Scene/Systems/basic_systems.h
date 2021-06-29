#pragma once
#include "ECS.h"

namespace spkt {

void script_system(spkt::registry& registry, double dt);
void animation_system(spkt::registry& registry, double dt);
void camera_system(spkt::registry& registry, double dt);
void path_follower_system(spkt::registry& registry, double dt);
void delete_below_50_system(spkt::registry& registry, double dt);
void clear_events_system(spkt::registry& registry, double dt);

}