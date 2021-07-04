#pragma once
#include "apecs.hpp"

namespace spkt {

void script_system(apx::registry& registry, double dt);
void animation_system(apx::registry& registry, double dt);
void camera_system(apx::registry& registry, double dt);
void path_follower_system(apx::registry& registry, double dt);
void delete_below_50_system(apx::registry& registry, double dt);
void clear_events_system(apx::registry& registry, double dt);

}