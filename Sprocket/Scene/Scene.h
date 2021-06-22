#pragma once
#include "ECS.h"
#include "Components.h"
#include "EntitySystem.h"
#include "Events.h"
#include "TypeInfo.h"

#include <memory>
#include <vector>
#include <unordered_map>
#include <string_view>

namespace Sprocket {

class Scene
{
    std::unordered_map<::spkt::type_info_t, std::size_t> d_lookup;
    std::vector<std::unique_ptr<EntitySystem>> d_systems;

    spkt::registry d_registry;

public:
    Scene();

    spkt::registry& Entities() { return d_registry; }

    template <typename T, typename... Args>
    T& Add(Args&&... args);

    template <typename T> bool Has();
    template <typename T> T& Get();

    void Load(std::string_view file);

    void OnUpdate(double dt);
    void OnEvent(ev::Event& event);

    std::size_t Size() const;

    void Clear();

    template <typename... Comps>
    spkt::entity find(const std::function<bool(spkt::entity)>& function = [](spkt::entity) { return true; });

    template <typename... Comps>
    apx::generator<spkt::entity> view();
};

template <typename T, typename... Args>
T& Scene::Add(Args&&... args)
{
    assert(d_registry.size() == 0);
    assert(d_lookup.find(spkt::type_info<T>) == d_lookup.end());
    d_lookup[spkt::type_info<T>] = d_systems.size();
    d_systems.emplace_back(std::make_unique<T>(std::forward<Args>(args)...));
    d_systems.back()->on_startup(d_registry);
    return *static_cast<T*>(d_systems.back().get());
}

template <typename T> bool Scene::Has()
{
    return d_lookup.find(spkt::type_info<T>) != d_lookup.end();
}

template <typename T> T& Scene::Get()
{
    auto it = d_lookup.find(spkt::type_info<T>);
    assert(it != d_lookup.end());
    return *static_cast<T*>(d_systems[it->second].get());
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
    if (sizeof...(Comps) > 0) {
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
