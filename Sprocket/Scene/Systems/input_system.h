#pragma once
#include "Events.h"
#include "apecs.hpp"

namespace spkt {

// This is a special system which is designed to get input information (keyboard,
// mouse and window) into the ECS for systems to make use of. As such, it requires
// a function that hooks into the event loop. Also, because of frame sensitive information
// such as mouse offsets, this requires two "system" functions, a begin and end.

// Processes events to build up the InputSingleton for the current frame.
void input_system_on_event(apx::registry& registry, ev::Event& event);

// Sets any per-frame information. Currently, this is just the mouse offset. This should be
// the first system added to any scene.
void input_system_begin(apx::registry& registry, double dt);

// Resets are per-frame information such as how much the mouse was scrolled or which
// button were pressed this frame. This should be the last system added to any scene.
void input_system_end(apx::registry& registry, double dt);

}