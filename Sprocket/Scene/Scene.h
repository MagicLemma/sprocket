// GENERATED FILE
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

#include "apecs.hpp"

namespace spkt {

// TODO: Allow for plugins to access the entire spec, so that we can have a
// {{Comp.Util.comma}} plugin which does not put the comma on the end. We currently
// have to remove this manually after generating. If the plugin could see the whole spec
// it would know if the current component is the last one.
using registry = apx::registry<
    Sprocket::TemporaryComponent,
    Sprocket::NameComponent,
    Sprocket::Transform2DComponent,
    Sprocket::Transform3DComponent,
    Sprocket::ModelComponent,
    Sprocket::RigidBody3DComponent,
    Sprocket::BoxCollider3DComponent,
    Sprocket::SphereCollider3DComponent,
    Sprocket::CapsuleCollider3DComponent,
    Sprocket::ScriptComponent,
    Sprocket::Camera3DComponent,
    Sprocket::SelectComponent,
    Sprocket::PathComponent,
    Sprocket::GridComponent,
    Sprocket::LightComponent,
    Sprocket::SunComponent,
    Sprocket::AmbienceComponent,
    Sprocket::ParticleComponent,
    Sprocket::MeshAnimationComponent,
    Sprocket::ParentComponent
>;

using entity = typename registry::handle_type;

}

namespace Sprocket {

class Scene
{
    std::unordered_map<spkt::type_info_t, std::size_t> d_lookup;
    std::vector<std::unique_ptr<EntitySystem>> d_systems;

    ecs::Registry d_registry;

    ev::Dispatcher d_dispatcher;

public:
    Scene();

    ecs::Registry& Entities() { return d_registry; }

    template <typename T, typename... Args>
    T& Add(Args&&... args);

    template <typename T> bool Has();
    template <typename T> T& Get();

    void Load(std::string_view file);

    void OnUpdate(double dt);
    void OnEvent(ev::Event& event);

    std::size_t Size() const;

    void Clear();
};

template <typename T, typename... Args>
T& Scene::Add(Args&&... args)
{
    assert(d_registry.size() == 0);
    assert(d_lookup.find(spkt::type_info<T>) == d_lookup.end());
    d_lookup[spkt::type_info<T>] = d_systems.size();
    d_systems.emplace_back(std::make_unique<T>(std::forward<Args>(args)...));
    d_systems.back()->on_startup(d_registry, d_dispatcher);
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

}
