#pragma once
#include <Sprocket/Scene/ecs.h>

#include <memory>
#include <vector>
#include <functional>

namespace spkt {

class event;

struct scene
{
    using system        = std::function<void(spkt::registry&, double)>;
    using event_handler = std::function<void(spkt::registry&, spkt::event&)>;

    spkt::registry             registry;
    std::vector<system>        systems;
    std::vector<event_handler> event_handlers;

    void on_update(double dt);
    void on_event(spkt::event& event);
};

}
