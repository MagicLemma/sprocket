#pragma once
#include <Sprocket/Scene/ecs.h>

namespace spkt {

void script_system(spkt::registry& registry, double dt);
void animation_system(spkt::registry& registry, double dt);
void camera_system(spkt::registry& registry, double dt);
void clear_events_system(spkt::registry& registry, double dt);

}