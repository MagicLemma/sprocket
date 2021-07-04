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
    // Temporary, will be removed when then the InputSingleton gets updated via a system.
    Window* d_window;

    apx::registry d_registry;
    std::vector<system_t> d_systems;

public:
    Scene(Window* window);
    ~Scene();

    apx::registry& Entities() { return d_registry; }

    void add(const system_t& system);

    void Load(std::string_view file);

    void on_update(double dt);
    void on_event(ev::Event& event);
};

}
