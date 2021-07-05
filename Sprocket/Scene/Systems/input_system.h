#pragma once
#include "Events.h"
#include "apecs.hpp"

namespace spkt {

void input_system_on_event(apx::registry& registry, ev::Event& event);
void input_system_begin(apx::registry& registry, double dt);
void input_system_end(apx::registry& registry, double dt);

}