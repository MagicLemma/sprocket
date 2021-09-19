#pragma once
#include <Sprocket/Scene/ecs.h>
#include <Sprocket/Core/window.h>

#include <memory>
#include <vector>
#include <functional>

namespace spkt {

class event;

spkt::entity add_singleton(spkt::registry& registry, const spkt::window* window);

template <typename Comp>
Comp& get_singleton(spkt::registry& registry)
{
    auto singleton = registry.find<Singleton>();
    assert(registry.valid(singleton));
    assert(registry.has<Comp>(singleton));
    return registry.get<Comp>(singleton);
}

struct scene
{
    using system = std::function<void(spkt::registry&, double)>;

    spkt::registry      registry;
    std::vector<system> systems;

    void on_update(double dt);
    void on_event(spkt::event& event);
};

}
