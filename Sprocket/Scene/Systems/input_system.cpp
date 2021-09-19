#include "input_system.h"

#include <Sprocket/Core/events.h>
#include <Sprocket/Scene/ecs.h>
#include <Sprocket/Scene/scene.h>
#include <Sprocket/Utility/input_store.h>

namespace spkt {

void input_system_on_event(spkt::registry& registry, spkt::event& event)
{
    auto& input = get_singleton<InputSingleton>(registry);
    input.input_store->on_event(event);
}

void input_system_end(spkt::registry& registry, double dt)
{
    auto& input = get_singleton<InputSingleton>(registry);
    input.input_store->end_frame();
}
    
}