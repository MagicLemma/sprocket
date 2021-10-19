#pragma once
#include <glm/glm.hpp>

namespace spkt {
    
void set_listener(const glm::vec3& postiion, const glm::vec3& direction);

float get_master_volume();
void set_master_volume(float volume);

}