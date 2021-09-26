#include "scene.h"

#include <Sprocket/Core/window.h>
#include <Sprocket/Core/events.h>
#include <Sprocket/Scene/ecs.h>
#include <Sprocket/Utility/input_store.h>

#include <memory>
#include <ranges>

namespace spkt {

spkt::entity add_singleton(spkt::registry& registry, const spkt::window* window)
{
    auto singleton = registry.create();
    registry.emplace<Runtime>(singleton);
    registry.emplace<Singleton>(singleton);
    registry.emplace<NameComponent>(singleton, "::RuntimeSingleton");
    auto& is = registry.emplace<InputSingleton>(singleton);
    is.input_store = std::make_shared<spkt::input_store>(window);
    return singleton;
}

void scene::on_update(double dt)
{
    std::ranges::for_each(systems, [&](const system& sys) {
        sys(registry, dt);
    });
}

void scene::on_event(spkt::event& event)
{
    std::ranges::for_each(event_handlers, [&](const event_handler& handler) {
        handler(registry, event);
    });
}

}
