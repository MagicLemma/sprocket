#pragma once
#include "apecs.hpp"

namespace spkt {
    
void set_listener(apx::registry& registry, apx::entity entity);

float get_master_volume();
void set_master_volume(float volume);

}