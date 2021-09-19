#pragma once
#include <Sprocket/Scene/ecs.h>

#include <string_view>

namespace spkt {

template <typename T>
struct reflection;

template <>
struct reflection<Runtime>
{
    static constexpr std::string_view component_name = "Runtime";
};

template <>
struct reflection<Singleton>
{
    static constexpr std::string_view component_name = "Singleton";
};

template <>
struct reflection<Event>
{
    static constexpr std::string_view component_name = "Event";
};

template <>
struct reflection<NameComponent>
{
    static constexpr std::string_view component_name = "NameComponent";
};

template <>
struct reflection<Transform2DComponent>
{
    static constexpr std::string_view component_name = "Transform2DComponent";
};

template <>
struct reflection<Transform3DComponent>
{
    static constexpr std::string_view component_name = "Transform3DComponent";
};

template <>
struct reflection<StaticModelComponent>
{
    static constexpr std::string_view component_name = "StaticModelComponent";
};

template <>
struct reflection<AnimatedModelComponent>
{
    static constexpr std::string_view component_name = "AnimatedModelComponent";
};

template <>
struct reflection<RigidBody3DComponent>
{
    static constexpr std::string_view component_name = "RigidBody3DComponent";
};

template <>
struct reflection<BoxCollider3DComponent>
{
    static constexpr std::string_view component_name = "BoxCollider3DComponent";
};

template <>
struct reflection<SphereCollider3DComponent>
{
    static constexpr std::string_view component_name = "SphereCollider3DComponent";
};

template <>
struct reflection<CapsuleCollider3DComponent>
{
    static constexpr std::string_view component_name = "CapsuleCollider3DComponent";
};

template <>
struct reflection<ScriptComponent>
{
    static constexpr std::string_view component_name = "ScriptComponent";
};

template <>
struct reflection<Camera3DComponent>
{
    static constexpr std::string_view component_name = "Camera3DComponent";
};

template <>
struct reflection<PathComponent>
{
    static constexpr std::string_view component_name = "PathComponent";
};

template <>
struct reflection<LightComponent>
{
    static constexpr std::string_view component_name = "LightComponent";
};

template <>
struct reflection<SunComponent>
{
    static constexpr std::string_view component_name = "SunComponent";
};

template <>
struct reflection<AmbienceComponent>
{
    static constexpr std::string_view component_name = "AmbienceComponent";
};

template <>
struct reflection<ParticleComponent>
{
    static constexpr std::string_view component_name = "ParticleComponent";
};

template <>
struct reflection<CollisionEvent>
{
    static constexpr std::string_view component_name = "CollisionEvent";
};

template <>
struct reflection<PhysicsSingleton>
{
    static constexpr std::string_view component_name = "PhysicsSingleton";
};

template <>
struct reflection<InputSingleton>
{
    static constexpr std::string_view component_name = "InputSingleton";
};

template <>
struct reflection<GameGridSingleton>
{
    static constexpr std::string_view component_name = "GameGridSingleton";
};

template <>
struct reflection<TileMapSingleton>
{
    static constexpr std::string_view component_name = "TileMapSingleton";
};

template <>
struct reflection<CameraSingleton>
{
    static constexpr std::string_view component_name = "CameraSingleton";
};

template <>
struct reflection<ParticleSingleton>
{
    static constexpr std::string_view component_name = "ParticleSingleton";
};


template <typename Func>
void for_each_comp_type(Func&& func)
{
    func(reflection<Runtime>{});
    func(reflection<Singleton>{});
    func(reflection<Event>{});
    func(reflection<NameComponent>{});
    func(reflection<Transform2DComponent>{});
    func(reflection<Transform3DComponent>{});
    func(reflection<StaticModelComponent>{});
    func(reflection<AnimatedModelComponent>{});
    func(reflection<RigidBody3DComponent>{});
    func(reflection<BoxCollider3DComponent>{});
    func(reflection<SphereCollider3DComponent>{});
    func(reflection<CapsuleCollider3DComponent>{});
    func(reflection<ScriptComponent>{});
    func(reflection<Camera3DComponent>{});
    func(reflection<PathComponent>{});
    func(reflection<LightComponent>{});
    func(reflection<SunComponent>{});
    func(reflection<AmbienceComponent>{});
    func(reflection<ParticleComponent>{});
    func(reflection<CollisionEvent>{});
    func(reflection<PhysicsSingleton>{});
    func(reflection<InputSingleton>{});
    func(reflection<GameGridSingleton>{});
    func(reflection<TileMapSingleton>{});
    func(reflection<CameraSingleton>{});
    func(reflection<ParticleSingleton>{});
}

}
