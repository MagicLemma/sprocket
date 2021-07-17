#pragma once
#include <Sprocket/Scene/ecs.h>

#include <memory>
#include <vector>
#include <functional>

namespace spkt {

class event;

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

    spkt::registry& registry() { return d_registry; }

    void add(const system_t& system);
    void on_update(double dt);
    void on_event(spkt::event& event);
};

}
