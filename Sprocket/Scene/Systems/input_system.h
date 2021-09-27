#pragma once
#include <Sprocket/Core/window.h>
#include <Sprocket/Core/events.h>
#include <Sprocket/Scene/ecs.h>

namespace spkt {

// Initialises the input singleton.
void input_system_init(spkt::registry& regsitry, spkt::window* window);

// Processes events to build up the InputSingleton for the current frame.
void input_system_on_event(spkt::registry& registry, spkt::event& event);

// Resets are per-frame information such as how much the mouse was scrolled or which
// button were pressed this frame. This should be the last system added to any scene.
void input_system_end(spkt::registry& registry, double dt);

}