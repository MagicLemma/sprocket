#include "scene.h"

#include <Sprocket/Core/events.h>
#include <Sprocket/Scene/ecs.h>
#include <Sprocket/Utility/input_store.h>

#include <memory>
#include <ranges>

namespace spkt {

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
