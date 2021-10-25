#include "scene.h"
#include "ecs.h"

#include <sprocket/core/events.h>

#include <memory>
#include <ranges>

namespace {{Global::namespace}} {

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
