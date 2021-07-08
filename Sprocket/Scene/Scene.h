#pragma once
#include "apecs.hpp"
#include "Components.h"
#include "Events.h"
#include "Window.h"

#include <memory>
#include <vector>
#include <string_view>
#include <functional>

namespace spkt {

apx::entity add_singleton(apx::registry& registry);

template <typename Comp>
Comp& get_singleton(apx::registry& registry)
{
    auto singleton = registry.find<Singleton>();
    assert(registry.valid(singleton));
    assert(registry.has<Comp>(singleton));
    return registry.get<Comp>(singleton);
}

class Scene
{
public:
    using system_t = std::function<void(apx::registry&, double)>;

private:
    apx::registry d_registry;
    std::vector<system_t> d_systems;

public:
    ~Scene();

    apx::registry& Entities() { return d_registry; }

    void add(const system_t& system);
    void on_update(double dt);
    void on_event(ev::Event& event);
};

}
