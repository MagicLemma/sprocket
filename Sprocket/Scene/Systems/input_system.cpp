#include "input_system.h"

#include <Sprocket/Core/events.h>
#include <Sprocket/Scene/ecs.h>
#include <Sprocket/Scene/scene.h>
#include <Sprocket/Utility/input_store.h>

#include <cassert>
#include <memory>

namespace spkt {

void input_system_init(spkt::registry& registry, spkt::window* window)
{
    assert(window);
    auto singleton = registry.find<Singleton>();
    auto& is = registry.emplace<InputSingleton>(singleton);
    is.input_store = std::make_shared<spkt::input_store>(window);
}

void input_system_on_event(spkt::registry& registry, spkt::event& event)
{
    get_singleton<InputSingleton>(registry).input_store->on_event(event);
}

void input_system_end(spkt::registry& registry, double dt)
{
    get_singleton<InputSingleton>(registry).input_store->end_frame();
}
    
}