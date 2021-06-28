#pragma once
#include "ECS.h"
#include "Components.h"
#include "EntitySystem.h"
#include "Events.h"
#include "Window.h"

#include <memory>
#include <vector>
#include <string_view>

namespace spkt {

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
    // Temporary, will be removed when then the InputSingleton gets updated via a system.
    Window* d_window;

    std::vector<std::unique_ptr<EntitySystem>> d_systems;

    spkt::registry d_registry;

public:
    Scene(Window* window);
    ~Scene();

    spkt::registry& Entities() { return d_registry; }

    template <typename T, typename... Args>
    T& Add(Args&&... args);

    void Load(std::string_view file);

    void OnUpdate(double dt);
    void OnEvent(ev::Event& event);

    std::size_t Size() const;

    template <typename... Comps>
    spkt::entity find(const std::function<bool(spkt::entity)>& function = [](spkt::entity) { return true; });

    template <typename... Comps>
    apx::generator<spkt::entity> view();
};

template <typename T, typename... Args>
T& Scene::Add(Args&&... args)
{
    d_systems.emplace_back(std::make_unique<T>(std::forward<Args>(args)...));
    d_systems.back()->on_startup(d_registry);
    return *static_cast<T*>(d_systems.back().get());
}

template <typename... Comps>
spkt::entity Scene::find(const std::function<bool(spkt::entity)>& function)
{
    for (auto entity : view<Comps...>()) {
        if (function(entity)) { return entity; }
    }
    return spkt::null;
}

template <typename... Comps>
apx::generator<spkt::entity> Scene::view()
{
    if constexpr (sizeof...(Comps) > 0) {
        for (auto id : d_registry.view<Comps...>()) {
            co_yield {d_registry, id};
        }
    } else {
        for (auto id : d_registry.all()) {
            co_yield {d_registry, id};
        }
    }
}

}
