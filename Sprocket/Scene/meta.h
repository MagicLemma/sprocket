#pragma once
#include <Sprocket/Scene/ecs.h>

#include <string_view>
#include <string>

namespace spkt {

template <typename T>
struct attribute_reflection
{
    const std::string_view name;
    const bool             is_savable;
    const bool             is_scriptable;

    const T* const         value;
};

template <typename T>
struct reflection;

template <>
struct reflection<Runtime>
{
    static constexpr const char* name          = "Runtime";
    static constexpr const char* display_name  = "Runtime";
    static constexpr bool        is_savable    = false;
    static constexpr bool        is_scriptable = false;

    template <typename Func>
    void attributes(Func&& func, const Runtime& component)
    {
    }
};

template <>
struct reflection<Singleton>
{
    static constexpr const char* name          = "Singleton";
    static constexpr const char* display_name  = "Singleton";
    static constexpr bool        is_savable    = false;
    static constexpr bool        is_scriptable = false;

    template <typename Func>
    void attributes(Func&& func, const Singleton& component)
    {
    }
};

template <>
struct reflection<Event>
{
    static constexpr const char* name          = "Event";
    static constexpr const char* display_name  = "Event";
    static constexpr bool        is_savable    = false;
    static constexpr bool        is_scriptable = false;

    template <typename Func>
    void attributes(Func&& func, const Event& component)
    {
    }
};

template <>
struct reflection<NameComponent>
{
    static constexpr const char* name          = "NameComponent";
    static constexpr const char* display_name  = "Name";
    static constexpr bool        is_savable    = true;
    static constexpr bool        is_scriptable = true;

    template <typename Func>
    void attributes(Func&& func, const NameComponent& component)
    {
        func(attribute_reflection<std::string>{.name="name", .is_savable=true, .is_scriptable=true, .value=&component.name});
    }
};

template <>
struct reflection<Transform2DComponent>
{
    static constexpr const char* name          = "Transform2DComponent";
    static constexpr const char* display_name  = "Transform 2D";
    static constexpr bool        is_savable    = true;
    static constexpr bool        is_scriptable = true;

    template <typename Func>
    void attributes(Func&& func, const Transform2DComponent& component)
    {
        func(attribute_reflection<glm::vec2>{.name="position", .is_savable=true, .is_scriptable=true, .value=&component.position});
        func(attribute_reflection<float>{.name="rotation", .is_savable=true, .is_scriptable=true, .value=&component.rotation});
        func(attribute_reflection<glm::vec2>{.name="scale", .is_savable=true, .is_scriptable=true, .value=&component.scale});
    }
};

template <>
struct reflection<Transform3DComponent>
{
    static constexpr const char* name          = "Transform3DComponent";
    static constexpr const char* display_name  = "Transform 3D";
    static constexpr bool        is_savable    = true;
    static constexpr bool        is_scriptable = true;

    template <typename Func>
    void attributes(Func&& func, const Transform3DComponent& component)
    {
        func(attribute_reflection<glm::vec3>{.name="position", .is_savable=true, .is_scriptable=true, .value=&component.position});
        func(attribute_reflection<glm::quat>{.name="orientation", .is_savable=true, .is_scriptable=false, .value=&component.orientation});
        func(attribute_reflection<glm::vec3>{.name="scale", .is_savable=true, .is_scriptable=true, .value=&component.scale});
    }
};

template <>
struct reflection<StaticModelComponent>
{
    static constexpr const char* name          = "StaticModelComponent";
    static constexpr const char* display_name  = "Static Model";
    static constexpr bool        is_savable    = true;
    static constexpr bool        is_scriptable = true;

    template <typename Func>
    void attributes(Func&& func, const StaticModelComponent& component)
    {
        func(attribute_reflection<std::string>{.name="mesh", .is_savable=true, .is_scriptable=true, .value=&component.mesh});
        func(attribute_reflection<std::string>{.name="material", .is_savable=true, .is_scriptable=true, .value=&component.material});
    }
};

template <>
struct reflection<AnimatedModelComponent>
{
    static constexpr const char* name          = "AnimatedModelComponent";
    static constexpr const char* display_name  = "Animated Model";
    static constexpr bool        is_savable    = true;
    static constexpr bool        is_scriptable = true;

    template <typename Func>
    void attributes(Func&& func, const AnimatedModelComponent& component)
    {
        func(attribute_reflection<std::string>{.name="mesh", .is_savable=true, .is_scriptable=true, .value=&component.mesh});
        func(attribute_reflection<std::string>{.name="material", .is_savable=true, .is_scriptable=true, .value=&component.material});
        func(attribute_reflection<std::string>{.name="animation_name", .is_savable=true, .is_scriptable=true, .value=&component.animation_name});
        func(attribute_reflection<float>{.name="animation_time", .is_savable=true, .is_scriptable=true, .value=&component.animation_time});
        func(attribute_reflection<float>{.name="animation_speed", .is_savable=true, .is_scriptable=true, .value=&component.animation_speed});
    }
};

template <>
struct reflection<RigidBody3DComponent>
{
    static constexpr const char* name          = "RigidBody3DComponent";
    static constexpr const char* display_name  = "Rigid Body 3D";
    static constexpr bool        is_savable    = true;
    static constexpr bool        is_scriptable = true;

    template <typename Func>
    void attributes(Func&& func, const RigidBody3DComponent& component)
    {
        func(attribute_reflection<glm::vec3>{.name="velocity", .is_savable=true, .is_scriptable=true, .value=&component.velocity});
        func(attribute_reflection<bool>{.name="gravity", .is_savable=true, .is_scriptable=true, .value=&component.gravity});
        func(attribute_reflection<bool>{.name="frozen", .is_savable=true, .is_scriptable=true, .value=&component.frozen});
        func(attribute_reflection<float>{.name="bounciness", .is_savable=true, .is_scriptable=true, .value=&component.bounciness});
        func(attribute_reflection<float>{.name="frictionCoefficient", .is_savable=true, .is_scriptable=true, .value=&component.frictionCoefficient});
        func(attribute_reflection<float>{.name="rollingResistance", .is_savable=true, .is_scriptable=true, .value=&component.rollingResistance});
        func(attribute_reflection<glm::vec3>{.name="force", .is_savable=false, .is_scriptable=true, .value=&component.force});
        func(attribute_reflection<bool>{.name="onFloor", .is_savable=false, .is_scriptable=true, .value=&component.onFloor});
        func(attribute_reflection<std::shared_ptr<rigid_body_runtime>>{.name="runtime", .is_savable=false, .is_scriptable=false, .value=&component.runtime});
    }
};

template <>
struct reflection<BoxCollider3DComponent>
{
    static constexpr const char* name          = "BoxCollider3DComponent";
    static constexpr const char* display_name  = "Box Collider 3D";
    static constexpr bool        is_savable    = true;
    static constexpr bool        is_scriptable = true;

    template <typename Func>
    void attributes(Func&& func, const BoxCollider3DComponent& component)
    {
        func(attribute_reflection<glm::vec3>{.name="position", .is_savable=true, .is_scriptable=true, .value=&component.position});
        func(attribute_reflection<glm::quat>{.name="orientation", .is_savable=true, .is_scriptable=false, .value=&component.orientation});
        func(attribute_reflection<float>{.name="mass", .is_savable=true, .is_scriptable=true, .value=&component.mass});
        func(attribute_reflection<glm::vec3>{.name="halfExtents", .is_savable=true, .is_scriptable=true, .value=&component.halfExtents});
        func(attribute_reflection<bool>{.name="applyScale", .is_savable=true, .is_scriptable=true, .value=&component.applyScale});
        func(attribute_reflection<std::shared_ptr<collider_runtime>>{.name="runtime", .is_savable=false, .is_scriptable=false, .value=&component.runtime});
    }
};

template <>
struct reflection<SphereCollider3DComponent>
{
    static constexpr const char* name          = "SphereCollider3DComponent";
    static constexpr const char* display_name  = "Sphere Collider 3D";
    static constexpr bool        is_savable    = true;
    static constexpr bool        is_scriptable = true;

    template <typename Func>
    void attributes(Func&& func, const SphereCollider3DComponent& component)
    {
        func(attribute_reflection<glm::vec3>{.name="position", .is_savable=true, .is_scriptable=true, .value=&component.position});
        func(attribute_reflection<glm::quat>{.name="orientation", .is_savable=true, .is_scriptable=false, .value=&component.orientation});
        func(attribute_reflection<float>{.name="mass", .is_savable=true, .is_scriptable=true, .value=&component.mass});
        func(attribute_reflection<float>{.name="radius", .is_savable=true, .is_scriptable=true, .value=&component.radius});
        func(attribute_reflection<std::shared_ptr<collider_runtime>>{.name="runtime", .is_savable=false, .is_scriptable=false, .value=&component.runtime});
    }
};

template <>
struct reflection<CapsuleCollider3DComponent>
{
    static constexpr const char* name          = "CapsuleCollider3DComponent";
    static constexpr const char* display_name  = "Capsule Collider 3D";
    static constexpr bool        is_savable    = true;
    static constexpr bool        is_scriptable = true;

    template <typename Func>
    void attributes(Func&& func, const CapsuleCollider3DComponent& component)
    {
        func(attribute_reflection<glm::vec3>{.name="position", .is_savable=true, .is_scriptable=true, .value=&component.position});
        func(attribute_reflection<glm::quat>{.name="orientation", .is_savable=true, .is_scriptable=false, .value=&component.orientation});
        func(attribute_reflection<float>{.name="mass", .is_savable=true, .is_scriptable=true, .value=&component.mass});
        func(attribute_reflection<float>{.name="radius", .is_savable=true, .is_scriptable=true, .value=&component.radius});
        func(attribute_reflection<float>{.name="height", .is_savable=true, .is_scriptable=true, .value=&component.height});
        func(attribute_reflection<std::shared_ptr<collider_runtime>>{.name="runtime", .is_savable=false, .is_scriptable=false, .value=&component.runtime});
    }
};

template <>
struct reflection<ScriptComponent>
{
    static constexpr const char* name          = "ScriptComponent";
    static constexpr const char* display_name  = "Script";
    static constexpr bool        is_savable    = true;
    static constexpr bool        is_scriptable = true;

    template <typename Func>
    void attributes(Func&& func, const ScriptComponent& component)
    {
        func(attribute_reflection<std::string>{.name="script", .is_savable=true, .is_scriptable=true, .value=&component.script});
        func(attribute_reflection<bool>{.name="active", .is_savable=true, .is_scriptable=true, .value=&component.active});
        func(attribute_reflection<std::shared_ptr<lua::script>>{.name="script_runtime", .is_savable=false, .is_scriptable=false, .value=&component.script_runtime});
    }
};

template <>
struct reflection<Camera3DComponent>
{
    static constexpr const char* name          = "Camera3DComponent";
    static constexpr const char* display_name  = "Camera 3D";
    static constexpr bool        is_savable    = true;
    static constexpr bool        is_scriptable = true;

    template <typename Func>
    void attributes(Func&& func, const Camera3DComponent& component)
    {
        func(attribute_reflection<glm::mat4>{.name="projection", .is_savable=false, .is_scriptable=false, .value=&component.projection});
        func(attribute_reflection<float>{.name="fov", .is_savable=true, .is_scriptable=true, .value=&component.fov});
        func(attribute_reflection<float>{.name="pitch", .is_savable=true, .is_scriptable=true, .value=&component.pitch});
    }
};

template <>
struct reflection<PathComponent>
{
    static constexpr const char* name          = "PathComponent";
    static constexpr const char* display_name  = "Path";
    static constexpr bool        is_savable    = true;
    static constexpr bool        is_scriptable = true;

    template <typename Func>
    void attributes(Func&& func, const PathComponent& component)
    {
        func(attribute_reflection<std::deque<glm::vec3>>{.name="markers", .is_savable=false, .is_scriptable=false, .value=&component.markers});
        func(attribute_reflection<float>{.name="speed", .is_savable=true, .is_scriptable=true, .value=&component.speed});
    }
};

template <>
struct reflection<LightComponent>
{
    static constexpr const char* name          = "LightComponent";
    static constexpr const char* display_name  = "Light";
    static constexpr bool        is_savable    = true;
    static constexpr bool        is_scriptable = true;

    template <typename Func>
    void attributes(Func&& func, const LightComponent& component)
    {
        func(attribute_reflection<glm::vec3>{.name="colour", .is_savable=true, .is_scriptable=true, .value=&component.colour});
        func(attribute_reflection<float>{.name="brightness", .is_savable=true, .is_scriptable=true, .value=&component.brightness});
    }
};

template <>
struct reflection<SunComponent>
{
    static constexpr const char* name          = "SunComponent";
    static constexpr const char* display_name  = "Sun";
    static constexpr bool        is_savable    = true;
    static constexpr bool        is_scriptable = true;

    template <typename Func>
    void attributes(Func&& func, const SunComponent& component)
    {
        func(attribute_reflection<glm::vec3>{.name="colour", .is_savable=true, .is_scriptable=true, .value=&component.colour});
        func(attribute_reflection<float>{.name="brightness", .is_savable=true, .is_scriptable=true, .value=&component.brightness});
        func(attribute_reflection<glm::vec3>{.name="direction", .is_savable=true, .is_scriptable=true, .value=&component.direction});
        func(attribute_reflection<bool>{.name="shadows", .is_savable=true, .is_scriptable=true, .value=&component.shadows});
    }
};

template <>
struct reflection<AmbienceComponent>
{
    static constexpr const char* name          = "AmbienceComponent";
    static constexpr const char* display_name  = "Ambience";
    static constexpr bool        is_savable    = true;
    static constexpr bool        is_scriptable = true;

    template <typename Func>
    void attributes(Func&& func, const AmbienceComponent& component)
    {
        func(attribute_reflection<glm::vec3>{.name="colour", .is_savable=true, .is_scriptable=true, .value=&component.colour});
        func(attribute_reflection<float>{.name="brightness", .is_savable=true, .is_scriptable=true, .value=&component.brightness});
    }
};

template <>
struct reflection<ParticleComponent>
{
    static constexpr const char* name          = "ParticleComponent";
    static constexpr const char* display_name  = "Particle";
    static constexpr bool        is_savable    = true;
    static constexpr bool        is_scriptable = true;

    template <typename Func>
    void attributes(Func&& func, const ParticleComponent& component)
    {
        func(attribute_reflection<float>{.name="interval", .is_savable=true, .is_scriptable=true, .value=&component.interval});
        func(attribute_reflection<glm::vec3>{.name="velocity", .is_savable=true, .is_scriptable=true, .value=&component.velocity});
        func(attribute_reflection<float>{.name="velocityNoise", .is_savable=true, .is_scriptable=true, .value=&component.velocityNoise});
        func(attribute_reflection<glm::vec3>{.name="acceleration", .is_savable=true, .is_scriptable=true, .value=&component.acceleration});
        func(attribute_reflection<glm::vec3>{.name="scale", .is_savable=true, .is_scriptable=true, .value=&component.scale});
        func(attribute_reflection<float>{.name="life", .is_savable=true, .is_scriptable=true, .value=&component.life});
        func(attribute_reflection<float>{.name="accumulator", .is_savable=false, .is_scriptable=false, .value=&component.accumulator});
    }
};

template <>
struct reflection<CollisionEvent>
{
    static constexpr const char* name          = "CollisionEvent";
    static constexpr const char* display_name  = "Collision Event";
    static constexpr bool        is_savable    = false;
    static constexpr bool        is_scriptable = true;

    template <typename Func>
    void attributes(Func&& func, const CollisionEvent& component)
    {
        func(attribute_reflection<spkt::entity>{.name="entity_a", .is_savable=true, .is_scriptable=true, .value=&component.entity_a});
        func(attribute_reflection<spkt::entity>{.name="entity_b", .is_savable=true, .is_scriptable=true, .value=&component.entity_b});
    }
};

template <>
struct reflection<PhysicsSingleton>
{
    static constexpr const char* name          = "PhysicsSingleton";
    static constexpr const char* display_name  = "Physics Singleton";
    static constexpr bool        is_savable    = false;
    static constexpr bool        is_scriptable = false;

    template <typename Func>
    void attributes(Func&& func, const PhysicsSingleton& component)
    {
        func(attribute_reflection<std::shared_ptr<physics_runtime>>{.name="physics_runtime", .is_savable=false, .is_scriptable=false, .value=&component.physics_runtime});
    }
};

template <>
struct reflection<InputSingleton>
{
    static constexpr const char* name          = "InputSingleton";
    static constexpr const char* display_name  = "Input Singleton";
    static constexpr bool        is_savable    = false;
    static constexpr bool        is_scriptable = false;

    template <typename Func>
    void attributes(Func&& func, const InputSingleton& component)
    {
        func(attribute_reflection<std::shared_ptr<spkt::input_store>>{.name="input_store", .is_savable=true, .is_scriptable=true, .value=&component.input_store});
    }
};

template <>
struct reflection<GameGridSingleton>
{
    static constexpr const char* name          = "GameGridSingleton";
    static constexpr const char* display_name  = "Game Grid Singleton";
    static constexpr bool        is_savable    = false;
    static constexpr bool        is_scriptable = false;

    template <typename Func>
    void attributes(Func&& func, const GameGridSingleton& component)
    {
        func(attribute_reflection<spkt::entity>{.name="hovered_square_entity", .is_savable=true, .is_scriptable=true, .value=&component.hovered_square_entity});
        func(attribute_reflection<spkt::entity>{.name="clicked_square_entity", .is_savable=true, .is_scriptable=true, .value=&component.clicked_square_entity});
        func(attribute_reflection<glm::ivec2>{.name="hovered_square", .is_savable=true, .is_scriptable=true, .value=&component.hovered_square});
        func(attribute_reflection<std::optional<glm::ivec2>>{.name="clicked_square", .is_savable=true, .is_scriptable=true, .value=&component.clicked_square});
    }
};

template <>
struct reflection<TileMapSingleton>
{
    static constexpr const char* name          = "TileMapSingleton";
    static constexpr const char* display_name  = "Tile Map Singleton";
    static constexpr bool        is_savable    = true;
    static constexpr bool        is_scriptable = false;

    template <typename Func>
    void attributes(Func&& func, const TileMapSingleton& component)
    {
        func(attribute_reflection<std::unordered_map<glm::ivec2, spkt::entity>>{.name="tiles", .is_savable=true, .is_scriptable=true, .value=&component.tiles});
    }
};

template <>
struct reflection<CameraSingleton>
{
    static constexpr const char* name          = "CameraSingleton";
    static constexpr const char* display_name  = "Camera Singleton";
    static constexpr bool        is_savable    = false;
    static constexpr bool        is_scriptable = false;

    template <typename Func>
    void attributes(Func&& func, const CameraSingleton& component)
    {
        func(attribute_reflection<spkt::entity>{.name="camera_entity", .is_savable=true, .is_scriptable=true, .value=&component.camera_entity});
    }
};

template <>
struct reflection<ParticleSingleton>
{
    static constexpr const char* name          = "ParticleSingleton";
    static constexpr const char* display_name  = "Particle Singleton";
    static constexpr bool        is_savable    = false;
    static constexpr bool        is_scriptable = false;

    template <typename Func>
    void attributes(Func&& func, const ParticleSingleton& component)
    {
        func(attribute_reflection<std::shared_ptr<std::array<particle, NUM_PARTICLES>>>{.name="particles", .is_savable=true, .is_scriptable=true, .value=&component.particles});
        func(attribute_reflection<std::size_t>{.name="next_slot", .is_savable=true, .is_scriptable=true, .value=&component.next_slot});
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
