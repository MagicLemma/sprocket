#pragma once
#include <Sprocket/Scene/ecs.h>

namespace spkt {
    
void set_listener(const spkt::registry& registry, spkt::entity entity);

float get_master_volume();
void set_master_volume(float volume);

}