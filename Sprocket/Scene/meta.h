#pragma once
#include <Sprocket/Scene/ecs.h>

#include <string_view>
#include <string>

namespace spkt {

template <typename T, bool Savable, bool Scriptable>
struct attribute_reflection
{
    const std::string_view name;
    const T* const         value;

    static constexpr bool  is_savable()    { return Savable; }
    static constexpr bool  is_scriptable() { return Scriptable; }
};

template <typename T>
struct reflection;

template <>
struct reflection<Runtime>
{
    static constexpr const char* name          = "Runtime";
    static constexpr const char* display_name  = "Runtime";

    static constexpr bool        is_savable()    { return false; }
    static constexpr bool        is_scriptable() { return false; }

    template <typename Func>
    void attributes(const Runtime& component, Func&& func)
    {
    }
};

template <>
struct reflection<Singleton>
{
    static constexpr const char* name          = "Singleton";
    static constexpr const char* display_name  = "Singleton";

    static constexpr bool        is_savable()    { return false; }
    static constexpr bool        is_scriptable() { return false; }

    template <typename Func>
    void attributes(const Singleton& component, Func&& func)
    {
    }
};

template <>
struct reflection<Event>
{
    static constexpr const char* name          = "Event";
    static constexpr const char* display_name  = "Event";

    static constexpr bool        is_savable()    { return false; }
    static constexpr bool        is_scriptable() { return false; }

    template <typename Func>
    void attributes(const Event& component, Func&& func)
    {
    }
};

template <>
struct reflection<NameComponent>
{
    static constexpr const char* name          = "NameComponent";
    static constexpr const char* display_name  = "Name";

    static constexpr bool        is_savable()    { return true; }
    static constexpr bool        is_scriptable() { return true; }

    template <typename Func>
    void attributes(const NameComponent& component, Func&& func)
    {
        func(attribute_reflection<std::string, true, true>{.name="name", .value=&component.name});
    }
};

template <>
struct reflection<Transform2DComponent>
{
    static constexpr const char* name          = "Transform2DComponent";
    static constexpr const char* display_name  = "Transform 2D";

    static constexpr bool        is_savable()    { return true; }
    static constexpr bool        is_scriptable() { return true; }

    template <typename Func>
    void attributes(const Transform2DComponent& component, Func&& func)
    {
        func(attribute_reflection<glm::vec2, true, true>{.name="position", .value=&component.position});
        func(attribute_reflection<float, true, true>{.name="rotation", .value=&component.rotation});
        func(attribute_reflection<glm::vec2, true, true>{.name="scale", .value=&component.scale});
    }
};

template <>
struct reflection<Transform3DComponent>
{
    static constexpr const char* name          = "Transform3DComponent";
    static constexpr const char* display_name  = "Transform 3D";

    static constexpr bool        is_savable()    { return true; }
    static constexpr bool        is_scriptable() { return true; }

    template <typename Func>
    void attributes(const Transform3DComponent& component, Func&& func)
    {
        func(attribute_reflection<glm::vec3, true, true>{.name="position", .value=&component.position});
        func(attribute_reflection<glm::quat, true, false>{.name="orientation", .value=&component.orientation});
        func(attribute_reflection<glm::vec3, true, true>{.name="scale", .value=&component.scale});
    }
};

template <>
struct reflection<StaticModelComponent>
{
    static constexpr const char* name          = "StaticModelComponent";
    static constexpr const char* display_name  = "Static Model";

    static constexpr bool        is_savable()    { return true; }
    static constexpr bool        is_scriptable() { return true; }

    template <typename Func>
    void attributes(const StaticModelComponent& component, Func&& func)
    {
        func(attribute_reflection<std::string, true, true>{.name="mesh", .value=&component.mesh});
        func(attribute_reflection<std::string, true, true>{.name="material", .value=&component.material});
    }
};

template <>
struct reflection<AnimatedModelComponent>
{
    static constexpr const char* name          = "AnimatedModelComponent";
    static constexpr const char* display_name  = "Animated Model";

    static constexpr bool        is_savable()    { return true; }
    static constexpr bool        is_scriptable() { return true; }

    template <typename Func>
    void attributes(const AnimatedModelComponent& component, Func&& func)
    {
        func(attribute_reflection<std::string, true, true>{.name="mesh", .value=&component.mesh});
        func(attribute_reflection<std::string, true, true>{.name="material", .value=&component.material});
        func(attribute_reflection<std::string, true, true>{.name="animation_name", .value=&component.animation_name});
        func(attribute_reflection<float, true, true>{.name="animation_time", .value=&component.animation_time});
        func(attribute_reflection<float, true, true>{.name="animation_speed", .value=&component.animation_speed});
    }
};

template <>
struct reflection<RigidBody3DComponent>
{
    static constexpr const char* name          = "RigidBody3DComponent";
    static constexpr const char* display_name  = "Rigid Body 3D";

    static constexpr bool        is_savable()    { return true; }
    static constexpr bool        is_scriptable() { return true; }

    template <typename Func>
    void attributes(const RigidBody3DComponent& component, Func&& func)
    {
        func(attribute_reflection<glm::vec3, true, true>{.name="velocity", .value=&component.velocity});
        func(attribute_reflection<bool, true, true>{.name="gravity", .value=&component.gravity});
        func(attribute_reflection<bool, true, true>{.name="frozen", .value=&component.frozen});
        func(attribute_reflection<float, true, true>{.name="bounciness", .value=&component.bounciness});
        func(attribute_reflection<float, true, true>{.name="frictionCoefficient", .value=&component.frictionCoefficient});
        func(attribute_reflection<float, true, true>{.name="rollingResistance", .value=&component.rollingResistance});
        func(attribute_reflection<glm::vec3, false, true>{.name="force", .value=&component.force});
        func(attribute_reflection<bool, false, true>{.name="onFloor", .value=&component.onFloor});
        func(attribute_reflection<std::shared_ptr<rigid_body_runtime>, false, false>{.name="runtime", .value=&component.runtime});
    }
};

template <>
struct reflection<BoxCollider3DComponent>
{
    static constexpr const char* name          = "BoxCollider3DComponent";
    static constexpr const char* display_name  = "Box Collider 3D";

    static constexpr bool        is_savable()    { return true; }
    static constexpr bool        is_scriptable() { return true; }

    template <typename Func>
    void attributes(const BoxCollider3DComponent& component, Func&& func)
    {
        func(attribute_reflection<glm::vec3, true, true>{.name="position", .value=&component.position});
        func(attribute_reflection<glm::quat, true, false>{.name="orientation", .value=&component.orientation});
        func(attribute_reflection<float, true, true>{.name="mass", .value=&component.mass});
        func(attribute_reflection<glm::vec3, true, true>{.name="halfExtents", .value=&component.halfExtents});
        func(attribute_reflection<bool, true, true>{.name="applyScale", .value=&component.applyScale});
        func(attribute_reflection<std::shared_ptr<collider_runtime>, false, false>{.name="runtime", .value=&component.runtime});
    }
};

template <>
struct reflection<SphereCollider3DComponent>
{
    static constexpr const char* name          = "SphereCollider3DComponent";
    static constexpr const char* display_name  = "Sphere Collider 3D";

    static constexpr bool        is_savable()    { return true; }
    static constexpr bool        is_scriptable() { return true; }

    template <typename Func>
    void attributes(const SphereCollider3DComponent& component, Func&& func)
    {
        func(attribute_reflection<glm::vec3, true, true>{.name="position", .value=&component.position});
        func(attribute_reflection<glm::quat, true, false>{.name="orientation", .value=&component.orientation});
        func(attribute_reflection<float, true, true>{.name="mass", .value=&component.mass});
        func(attribute_reflection<float, true, true>{.name="radius", .value=&component.radius});
        func(attribute_reflection<std::shared_ptr<collider_runtime>, false, false>{.name="runtime", .value=&component.runtime});
    }
};

template <>
struct reflection<CapsuleCollider3DComponent>
{
    static constexpr const char* name          = "CapsuleCollider3DComponent";
    static constexpr const char* display_name  = "Capsule Collider 3D";

    static constexpr bool        is_savable()    { return true; }
    static constexpr bool        is_scriptable() { return true; }

    template <typename Func>
    void attributes(const CapsuleCollider3DComponent& component, Func&& func)
    {
        func(attribute_reflection<glm::vec3, true, true>{.name="position", .value=&component.position});
        func(attribute_reflection<glm::quat, true, false>{.name="orientation", .value=&component.orientation});
        func(attribute_reflection<float, true, true>{.name="mass", .value=&component.mass});
        func(attribute_reflection<float, true, true>{.name="radius", .value=&component.radius});
        func(attribute_reflection<float, true, true>{.name="height", .value=&component.height});
        func(attribute_reflection<std::shared_ptr<collider_runtime>, false, false>{.name="runtime", .value=&component.runtime});
    }
};

template <>
struct reflection<ScriptComponent>
{
    static constexpr const char* name          = "ScriptComponent";
    static constexpr const char* display_name  = "Script";

    static constexpr bool        is_savable()    { return true; }
    static constexpr bool        is_scriptable() { return true; }

    template <typename Func>
    void attributes(const ScriptComponent& component, Func&& func)
    {
        func(attribute_reflection<std::string, true, true>{.name="script", .value=&component.script});
        func(attribute_reflection<bool, true, true>{.name="active", .value=&component.active});
        func(attribute_reflection<std::shared_ptr<lua::script>, false, false>{.name="script_runtime", .value=&component.script_runtime});
    }
};

template <>
struct reflection<Camera3DComponent>
{
    static constexpr const char* name          = "Camera3DComponent";
    static constexpr const char* display_name  = "Camera 3D";

    static constexpr bool        is_savable()    { return true; }
    static constexpr bool        is_scriptable() { return true; }

    template <typename Func>
    void attributes(const Camera3DComponent& component, Func&& func)
    {
        func(attribute_reflection<glm::mat4, false, false>{.name="projection", .value=&component.projection});
        func(attribute_reflection<float, true, true>{.name="fov", .value=&component.fov});
        func(attribute_reflection<float, true, true>{.name="pitch", .value=&component.pitch});
    }
};

template <>
struct reflection<PathComponent>
{
    static constexpr const char* name          = "PathComponent";
    static constexpr const char* display_name  = "Path";

    static constexpr bool        is_savable()    { return true; }
    static constexpr bool        is_scriptable() { return true; }

    template <typename Func>
    void attributes(const PathComponent& component, Func&& func)
    {
        func(attribute_reflection<std::deque<glm::vec3>, false, false>{.name="markers", .value=&component.markers});
        func(attribute_reflection<float, true, true>{.name="speed", .value=&component.speed});
    }
};

template <>
struct reflection<LightComponent>
{
    static constexpr const char* name          = "LightComponent";
    static constexpr const char* display_name  = "Light";

    static constexpr bool        is_savable()    { return true; }
    static constexpr bool        is_scriptable() { return true; }

    template <typename Func>
    void attributes(const LightComponent& component, Func&& func)
    {
        func(attribute_reflection<glm::vec3, true, true>{.name="colour", .value=&component.colour});
        func(attribute_reflection<float, true, true>{.name="brightness", .value=&component.brightness});
    }
};

template <>
struct reflection<SunComponent>
{
    static constexpr const char* name          = "SunComponent";
    static constexpr const char* display_name  = "Sun";

    static constexpr bool        is_savable()    { return true; }
    static constexpr bool        is_scriptable() { return true; }

    template <typename Func>
    void attributes(const SunComponent& component, Func&& func)
    {
        func(attribute_reflection<glm::vec3, true, true>{.name="colour", .value=&component.colour});
        func(attribute_reflection<float, true, true>{.name="brightness", .value=&component.brightness});
        func(attribute_reflection<glm::vec3, true, true>{.name="direction", .value=&component.direction});
        func(attribute_reflection<bool, true, true>{.name="shadows", .value=&component.shadows});
    }
};

template <>
struct reflection<AmbienceComponent>
{
    static constexpr const char* name          = "AmbienceComponent";
    static constexpr const char* display_name  = "Ambience";

    static constexpr bool        is_savable()    { return true; }
    static constexpr bool        is_scriptable() { return true; }

    template <typename Func>
    void attributes(const AmbienceComponent& component, Func&& func)
    {
        func(attribute_reflection<glm::vec3, true, true>{.name="colour", .value=&component.colour});
        func(attribute_reflection<float, true, true>{.name="brightness", .value=&component.brightness});
    }
};

template <>
struct reflection<ParticleComponent>
{
    static constexpr const char* name          = "ParticleComponent";
    static constexpr const char* display_name  = "Particle";

    static constexpr bool        is_savable()    { return true; }
    static constexpr bool        is_scriptable() { return true; }

    template <typename Func>
    void attributes(const ParticleComponent& component, Func&& func)
    {
        func(attribute_reflection<float, true, true>{.name="interval", .value=&component.interval});
        func(attribute_reflection<glm::vec3, true, true>{.name="velocity", .value=&component.velocity});
        func(attribute_reflection<float, true, true>{.name="velocityNoise", .value=&component.velocityNoise});
        func(attribute_reflection<glm::vec3, true, true>{.name="acceleration", .value=&component.acceleration});
        func(attribute_reflection<glm::vec3, true, true>{.name="scale", .value=&component.scale});
        func(attribute_reflection<float, true, true>{.name="life", .value=&component.life});
        func(attribute_reflection<float, false, false>{.name="accumulator", .value=&component.accumulator});
    }
};

template <>
struct reflection<CollisionEvent>
{
    static constexpr const char* name          = "CollisionEvent";
    static constexpr const char* display_name  = "Collision Event";

    static constexpr bool        is_savable()    { return false; }
    static constexpr bool        is_scriptable() { return true; }

    template <typename Func>
    void attributes(const CollisionEvent& component, Func&& func)
    {
        func(attribute_reflection<spkt::entity, true, true>{.name="entity_a", .value=&component.entity_a});
        func(attribute_reflection<spkt::entity, true, true>{.name="entity_b", .value=&component.entity_b});
    }
};

template <>
struct reflection<PhysicsSingleton>
{
    static constexpr const char* name          = "PhysicsSingleton";
    static constexpr const char* display_name  = "Physics Singleton";

    static constexpr bool        is_savable()    { return false; }
    static constexpr bool        is_scriptable() { return false; }

    template <typename Func>
    void attributes(const PhysicsSingleton& component, Func&& func)
    {
        func(attribute_reflection<std::shared_ptr<physics_runtime>, false, false>{.name="physics_runtime", .value=&component.physics_runtime});
    }
};

template <>
struct reflection<InputSingleton>
{
    static constexpr const char* name          = "InputSingleton";
    static constexpr const char* display_name  = "Input Singleton";

    static constexpr bool        is_savable()    { return false; }
    static constexpr bool        is_scriptable() { return false; }

    template <typename Func>
    void attributes(const InputSingleton& component, Func&& func)
    {
        func(attribute_reflection<std::shared_ptr<spkt::input_store>, true, true>{.name="input_store", .value=&component.input_store});
    }
};

template <>
struct reflection<GameGridSingleton>
{
    static constexpr const char* name          = "GameGridSingleton";
    static constexpr const char* display_name  = "Game Grid Singleton";

    static constexpr bool        is_savable()    { return false; }
    static constexpr bool        is_scriptable() { return false; }

    template <typename Func>
    void attributes(const GameGridSingleton& component, Func&& func)
    {
        func(attribute_reflection<spkt::entity, true, true>{.name="hovered_square_entity", .value=&component.hovered_square_entity});
        func(attribute_reflection<spkt::entity, true, true>{.name="clicked_square_entity", .value=&component.clicked_square_entity});
        func(attribute_reflection<glm::ivec2, true, true>{.name="hovered_square", .value=&component.hovered_square});
        func(attribute_reflection<std::optional<glm::ivec2>, true, true>{.name="clicked_square", .value=&component.clicked_square});
    }
};

template <>
struct reflection<TileMapSingleton>
{
    static constexpr const char* name          = "TileMapSingleton";
    static constexpr const char* display_name  = "Tile Map Singleton";

    static constexpr bool        is_savable()    { return true; }
    static constexpr bool        is_scriptable() { return false; }

    template <typename Func>
    void attributes(const TileMapSingleton& component, Func&& func)
    {
        func(attribute_reflection<std::unordered_map<glm::ivec2, spkt::entity>, true, true>{.name="tiles", .value=&component.tiles});
    }
};

template <>
struct reflection<CameraSingleton>
{
    static constexpr const char* name          = "CameraSingleton";
    static constexpr const char* display_name  = "Camera Singleton";

    static constexpr bool        is_savable()    { return false; }
    static constexpr bool        is_scriptable() { return false; }

    template <typename Func>
    void attributes(const CameraSingleton& component, Func&& func)
    {
        func(attribute_reflection<spkt::entity, true, true>{.name="camera_entity", .value=&component.camera_entity});
    }
};

template <>
struct reflection<ParticleSingleton>
{
    static constexpr const char* name          = "ParticleSingleton";
    static constexpr const char* display_name  = "Particle Singleton";

    static constexpr bool        is_savable()    { return false; }
    static constexpr bool        is_scriptable() { return false; }

    template <typename Func>
    void attributes(const ParticleSingleton& component, Func&& func)
    {
        func(attribute_reflection<std::shared_ptr<std::array<particle, NUM_PARTICLES>>, true, true>{.name="particles", .value=&component.particles});
        func(attribute_reflection<std::size_t, true, true>{.name="next_slot", .value=&component.next_slot});
    }
};


template <typename Func>
void for_each_reflect(Func&& func)
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
