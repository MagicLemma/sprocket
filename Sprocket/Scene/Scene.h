#pragma once
#include "ecs.h"
#include "ecs.h"
#include "Events.h"
#include "Window.h"

#include <memory>
#include <vector>
#include <string_view>
#include <functional>

namespace spkt {

spkt::entity add_singleton(spkt::registry& registry);

template <typename Comp>
Comp& get_singleton(spkt::registry& registry)
{
    auto singleton = registry.find<Singleton>();
    assert(registry.valid(singleton));
    assert(registry.has<Comp>(singleton));
    return registry.get<Comp>(singleton);
}

class Scene
{
public:
    using system_t = std::function<void(spkt::registry&, double)>;

private:
    spkt::registry d_registry;
    std::vector<system_t> d_systems;

public:
    ~Scene();

    spkt::registry& Entities() { return d_registry; }

    void add(const system_t& system);
    void on_update(double dt);
    void on_event(ev::Event& event);
};

}
