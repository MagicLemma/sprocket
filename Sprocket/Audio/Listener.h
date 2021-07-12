#pragma once
#include "ecs.h"

namespace spkt {
    
void set_listener(spkt::registry& registry, spkt::entity entity);

float get_master_volume();
void set_master_volume(float volume);

}