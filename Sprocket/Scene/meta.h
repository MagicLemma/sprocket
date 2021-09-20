#pragma once
#include <Sprocket/Scene/ecs.h>

#include <string_view>
#include <string>

namespace spkt {

template <typename T>
struct attribute_reflection
{
    const std::string_view name;
    const bool             savable;
    const bool             scriptable;
};

template <typename T>
struct reflection;

template <>
struct reflection<Runtime>
{
    static constexpr const char* name         = "Runtime";
    static constexpr const char* display_name = "Runtime";
    static constexpr bool        savable      = false;
    static constexpr bool        scriptable   = false;

    template <typename Func>
    void attributes(Func&& func)
    {
    }
};

template <>
struct reflection<Singleton>
{
    static constexpr const char* name         = "Singleton";
    static constexpr const char* display_name = "Singleton";
    static constexpr bool        savable      = false;
    static constexpr bool        scriptable   = false;

    template <typename Func>
    void attributes(Func&& func)
    {
    }
};

template <>
struct reflection<Event>
{
    static constexpr const char* name         = "Event";
    static constexpr const char* display_name = "Event";
    static constexpr bool        savable      = false;
    static constexpr bool        scriptable   = false;

    template <typename Func>
    void attributes(Func&& func)
    {
    }
};

template <>
struct reflection<NameComponent>
{
    static constexpr const char* name         = "NameComponent";
    static constexpr const char* display_name = "Name";
    static constexpr bool        savable      = true;
    static constexpr bool        scriptable   = true;

    template <typename Func>
    void attributes(Func&& func)
    {
        func(attribute_reflection<std::string>{.name="name", .savable=true, .scriptable=true});
    }
};

template <>
struct reflection<Transform2DComponent>
{
    static constexpr const char* name         = "Transform2DComponent";
    static constexpr const char* display_name = "Transform 2D";
    static constexpr bool        savable      = true;
    static constexpr bool        scriptable   = true;

    template <typename Func>
    void attributes(Func&& func)
    {
        func(attribute_reflection<glm::vec2>{.name="position", .savable=true, .scriptable=true});
        func(attribute_reflection<float>{.name="rotation", .savable=true, .scriptable=true});
        func(attribute_reflection<glm::vec2>{.name="scale", .savable=true, .scriptable=true});
    }
};

template <>
struct reflection<Transform3DComponent>
{
    static constexpr const char* name         = "Transform3DComponent";
    static constexpr const char* display_name = "Transform 3D";
    static constexpr bool        savable      = true;
    static constexpr bool        scriptable   = true;

    template <typename Func>
    void attributes(Func&& func)
    {
        func(attribute_reflection<glm::vec3>{.name="position", .savable=true, .scriptable=true});
        func(attribute_reflection<glm::quat>{.name="orientation", .savable=true, .scriptable=false});
        func(attribute_reflection<glm::vec3>{.name="scale", .savable=true, .scriptable=true});
    }
};

template <>
struct reflection<StaticModelComponent>
{
    static constexpr const char* name         = "StaticModelComponent";
    static constexpr const char* display_name = "Static Model";
    static constexpr bool        savable      = true;
    static constexpr bool        scriptable   = true;

    template <typename Func>
    void attributes(Func&& func)
    {
        func(attribute_reflection<std::string>{.name="mesh", .savable=true, .scriptable=true});
        func(attribute_reflection<std::string>{.name="material", .savable=true, .scriptable=true});
    }
};

template <>
struct reflection<AnimatedModelComponent>
{
    static constexpr const char* name         = "AnimatedModelComponent";
    static constexpr const char* display_name = "Animated Model";
    static constexpr bool        savable      = true;
    static constexpr bool        scriptable   = true;

    template <typename Func>
    void attributes(Func&& func)
    {
        func(attribute_reflection<std::string>{.name="mesh", .savable=true, .scriptable=true});
        func(attribute_reflection<std::string>{.name="material", .savable=true, .scriptable=true});
        func(attribute_reflection<std::string>{.name="animation_name", .savable=true, .scriptable=true});
        func(attribute_reflection<float>{.name="animation_time", .savable=true, .scriptable=true});
        func(attribute_reflection<float>{.name="animation_speed", .savable=true, .scriptable=true});
    }
};

template <>
struct reflection<RigidBody3DComponent>
{
    static constexpr const char* name         = "RigidBody3DComponent";
    static constexpr const char* display_name = "Rigid Body 3D";
    static constexpr bool        savable      = true;
    static constexpr bool        scriptable   = true;

    template <typename Func>
    void attributes(Func&& func)
    {
        func(attribute_reflection<glm::vec3>{.name="velocity", .savable=true, .scriptable=true});
        func(attribute_reflection<bool>{.name="gravity", .savable=true, .scriptable=true});
        func(attribute_reflection<bool>{.name="frozen", .savable=true, .scriptable=true});
        func(attribute_reflection<float>{.name="bounciness", .savable=true, .scriptable=true});
        func(attribute_reflection<float>{.name="frictionCoefficient", .savable=true, .scriptable=true});
        func(attribute_reflection<float>{.name="rollingResistance", .savable=true, .scriptable=true});
        func(attribute_reflection<glm::vec3>{.name="force", .savable=false, .scriptable=true});
        func(attribute_reflection<bool>{.name="onFloor", .savable=false, .scriptable=true});
        func(attribute_reflection<std::shared_ptr<rigid_body_runtime>>{.name="runtime", .savable=false, .scriptable=false});
    }
};

template <>
struct reflection<BoxCollider3DComponent>
{
    static constexpr const char* name         = "BoxCollider3DComponent";
    static constexpr const char* display_name = "Box Collider 3D";
    static constexpr bool        savable      = true;
    static constexpr bool        scriptable   = true;

    template <typename Func>
    void attributes(Func&& func)
    {
        func(attribute_reflection<glm::vec3>{.name="position", .savable=true, .scriptable=true});
        func(attribute_reflection<glm::quat>{.name="orientation", .savable=true, .scriptable=false});
        func(attribute_reflection<float>{.name="mass", .savable=true, .scriptable=true});
        func(attribute_reflection<glm::vec3>{.name="halfExtents", .savable=true, .scriptable=true});
        func(attribute_reflection<bool>{.name="applyScale", .savable=true, .scriptable=true});
        func(attribute_reflection<std::shared_ptr<collider_runtime>>{.name="runtime", .savable=false, .scriptable=false});
    }
};

template <>
struct reflection<SphereCollider3DComponent>
{
    static constexpr const char* name         = "SphereCollider3DComponent";
    static constexpr const char* display_name = "Sphere Collider 3D";
    static constexpr bool        savable      = true;
    static constexpr bool        scriptable   = true;

    template <typename Func>
    void attributes(Func&& func)
    {
        func(attribute_reflection<glm::vec3>{.name="position", .savable=true, .scriptable=true});
        func(attribute_reflection<glm::quat>{.name="orientation", .savable=true, .scriptable=false});
        func(attribute_reflection<float>{.name="mass", .savable=true, .scriptable=true});
        func(attribute_reflection<float>{.name="radius", .savable=true, .scriptable=true});
        func(attribute_reflection<std::shared_ptr<collider_runtime>>{.name="runtime", .savable=false, .scriptable=false});
    }
};

template <>
struct reflection<CapsuleCollider3DComponent>
{
    static constexpr const char* name         = "CapsuleCollider3DComponent";
    static constexpr const char* display_name = "Capsule Collider 3D";
    static constexpr bool        savable      = true;
    static constexpr bool        scriptable   = true;

    template <typename Func>
    void attributes(Func&& func)
    {
        func(attribute_reflection<glm::vec3>{.name="position", .savable=true, .scriptable=true});
        func(attribute_reflection<glm::quat>{.name="orientation", .savable=true, .scriptable=false});
        func(attribute_reflection<float>{.name="mass", .savable=true, .scriptable=true});
        func(attribute_reflection<float>{.name="radius", .savable=true, .scriptable=true});
        func(attribute_reflection<float>{.name="height", .savable=true, .scriptable=true});
        func(attribute_reflection<std::shared_ptr<collider_runtime>>{.name="runtime", .savable=false, .scriptable=false});
    }
};

template <>
struct reflection<ScriptComponent>
{
    static constexpr const char* name         = "ScriptComponent";
    static constexpr const char* display_name = "Script";
    static constexpr bool        savable      = true;
    static constexpr bool        scriptable   = true;

    template <typename Func>
    void attributes(Func&& func)
    {
        func(attribute_reflection<std::string>{.name="script", .savable=true, .scriptable=true});
        func(attribute_reflection<bool>{.name="active", .savable=true, .scriptable=true});
        func(attribute_reflection<std::shared_ptr<lua::script>>{.name="script_runtime", .savable=false, .scriptable=false});
    }
};

template <>
struct reflection<Camera3DComponent>
{
    static constexpr const char* name         = "Camera3DComponent";
    static constexpr const char* display_name = "Camera 3D";
    static constexpr bool        savable      = true;
    static constexpr bool        scriptable   = true;

    template <typename Func>
    void attributes(Func&& func)
    {
        func(attribute_reflection<glm::mat4>{.name="projection", .savable=false, .scriptable=false});
        func(attribute_reflection<float>{.name="fov", .savable=true, .scriptable=true});
        func(attribute_reflection<float>{.name="pitch", .savable=true, .scriptable=true});
    }
};

template <>
struct reflection<PathComponent>
{
    static constexpr const char* name         = "PathComponent";
    static constexpr const char* display_name = "Path";
    static constexpr bool        savable      = true;
    static constexpr bool        scriptable   = true;

    template <typename Func>
    void attributes(Func&& func)
    {
        func(attribute_reflection<std::deque<glm::vec3>>{.name="markers", .savable=false, .scriptable=false});
        func(attribute_reflection<float>{.name="speed", .savable=true, .scriptable=true});
    }
};

template <>
struct reflection<LightComponent>
{
    static constexpr const char* name         = "LightComponent";
    static constexpr const char* display_name = "Light";
    static constexpr bool        savable      = true;
    static constexpr bool        scriptable   = true;

    template <typename Func>
    void attributes(Func&& func)
    {
        func(attribute_reflection<glm::vec3>{.name="colour", .savable=true, .scriptable=true});
        func(attribute_reflection<float>{.name="brightness", .savable=true, .scriptable=true});
    }
};

template <>
struct reflection<SunComponent>
{
    static constexpr const char* name         = "SunComponent";
    static constexpr const char* display_name = "Sun";
    static constexpr bool        savable      = true;
    static constexpr bool        scriptable   = true;

    template <typename Func>
    void attributes(Func&& func)
    {
        func(attribute_reflection<glm::vec3>{.name="colour", .savable=true, .scriptable=true});
        func(attribute_reflection<float>{.name="brightness", .savable=true, .scriptable=true});
        func(attribute_reflection<glm::vec3>{.name="direction", .savable=true, .scriptable=true});
        func(attribute_reflection<bool>{.name="shadows", .savable=true, .scriptable=true});
    }
};

template <>
struct reflection<AmbienceComponent>
{
    static constexpr const char* name         = "AmbienceComponent";
    static constexpr const char* display_name = "Ambience";
    static constexpr bool        savable      = true;
    static constexpr bool        scriptable   = true;

    template <typename Func>
    void attributes(Func&& func)
    {
        func(attribute_reflection<glm::vec3>{.name="colour", .savable=true, .scriptable=true});
        func(attribute_reflection<float>{.name="brightness", .savable=true, .scriptable=true});
    }
};

template <>
struct reflection<ParticleComponent>
{
    static constexpr const char* name         = "ParticleComponent";
    static constexpr const char* display_name = "Particle";
    static constexpr bool        savable      = true;
    static constexpr bool        scriptable   = true;

    template <typename Func>
    void attributes(Func&& func)
    {
        func(attribute_reflection<float>{.name="interval", .savable=true, .scriptable=true});
        func(attribute_reflection<glm::vec3>{.name="velocity", .savable=true, .scriptable=true});
        func(attribute_reflection<float>{.name="velocityNoise", .savable=true, .scriptable=true});
        func(attribute_reflection<glm::vec3>{.name="acceleration", .savable=true, .scriptable=true});
        func(attribute_reflection<glm::vec3>{.name="scale", .savable=true, .scriptable=true});
        func(attribute_reflection<float>{.name="life", .savable=true, .scriptable=true});
        func(attribute_reflection<float>{.name="accumulator", .savable=false, .scriptable=false});
    }
};

template <>
struct reflection<CollisionEvent>
{
    static constexpr const char* name         = "CollisionEvent";
    static constexpr const char* display_name = "Collision Event";
    static constexpr bool        savable      = false;
    static constexpr bool        scriptable   = true;

    template <typename Func>
    void attributes(Func&& func)
    {
        func(attribute_reflection<spkt::entity>{.name="entity_a", .savable=true, .scriptable=true});
        func(attribute_reflection<spkt::entity>{.name="entity_b", .savable=true, .scriptable=true});
    }
};

template <>
struct reflection<PhysicsSingleton>
{
    static constexpr const char* name         = "PhysicsSingleton";
    static constexpr const char* display_name = "Physics Singleton";
    static constexpr bool        savable      = false;
    static constexpr bool        scriptable   = false;

    template <typename Func>
    void attributes(Func&& func)
    {
        func(attribute_reflection<std::shared_ptr<physics_runtime>>{.name="physics_runtime", .savable=false, .scriptable=false});
    }
};

template <>
struct reflection<InputSingleton>
{
    static constexpr const char* name         = "InputSingleton";
    static constexpr const char* display_name = "Input Singleton";
    static constexpr bool        savable      = false;
    static constexpr bool        scriptable   = false;

    template <typename Func>
    void attributes(Func&& func)
    {
        func(attribute_reflection<std::shared_ptr<spkt::input_store>>{.name="input_store", .savable=true, .scriptable=true});
    }
};

template <>
struct reflection<GameGridSingleton>
{
    static constexpr const char* name         = "GameGridSingleton";
    static constexpr const char* display_name = "Game Grid Singleton";
    static constexpr bool        savable      = false;
    static constexpr bool        scriptable   = false;

    template <typename Func>
    void attributes(Func&& func)
    {
        func(attribute_reflection<spkt::entity>{.name="hovered_square_entity", .savable=true, .scriptable=true});
        func(attribute_reflection<spkt::entity>{.name="clicked_square_entity", .savable=true, .scriptable=true});
        func(attribute_reflection<glm::ivec2>{.name="hovered_square", .savable=true, .scriptable=true});
        func(attribute_reflection<std::optional<glm::ivec2>>{.name="clicked_square", .savable=true, .scriptable=true});
    }
};

template <>
struct reflection<TileMapSingleton>
{
    static constexpr const char* name         = "TileMapSingleton";
    static constexpr const char* display_name = "Tile Map Singleton";
    static constexpr bool        savable      = true;
    static constexpr bool        scriptable   = false;

    template <typename Func>
    void attributes(Func&& func)
    {
        func(attribute_reflection<std::unordered_map<glm::ivec2, spkt::entity>>{.name="tiles", .savable=true, .scriptable=true});
    }
};

template <>
struct reflection<CameraSingleton>
{
    static constexpr const char* name         = "CameraSingleton";
    static constexpr const char* display_name = "Camera Singleton";
    static constexpr bool        savable      = false;
    static constexpr bool        scriptable   = false;

    template <typename Func>
    void attributes(Func&& func)
    {
        func(attribute_reflection<spkt::entity>{.name="camera_entity", .savable=true, .scriptable=true});
    }
};

template <>
struct reflection<ParticleSingleton>
{
    static constexpr const char* name         = "ParticleSingleton";
    static constexpr const char* display_name = "Particle Singleton";
    static constexpr bool        savable      = false;
    static constexpr bool        scriptable   = false;

    template <typename Func>
    void attributes(Func&& func)
    {
        func(attribute_reflection<std::shared_ptr<std::array<particle, NUM_PARTICLES>>>{.name="particles", .savable=true, .scriptable=true});
        func(attribute_reflection<std::size_t>{.name="next_slot", .savable=true, .scriptable=true});
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
