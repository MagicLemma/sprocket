#pragma once
#include <Sprocket/Scene/ecs.h>

#include <string_view>
#include <string>

namespace spkt {

template <typename T>
struct attribute_reflection
{
    const std::string_view name;
};

template <typename T>
struct reflection;

template <>
struct reflection<Runtime>
{
    static constexpr const char* name         = "Runtime";
    static constexpr const char* display_name = "Runtime";

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

    template <typename Func>
    void attributes(Func&& func)
    {
        func(attribute_reflection<std::string>{"name"});
    }
};

template <>
struct reflection<Transform2DComponent>
{
    static constexpr const char* name         = "Transform2DComponent";
    static constexpr const char* display_name = "Transform 2D";

    template <typename Func>
    void attributes(Func&& func)
    {
        func(attribute_reflection<glm::vec2>{"position"});
        func(attribute_reflection<float>{"rotation"});
        func(attribute_reflection<glm::vec2>{"scale"});
    }
};

template <>
struct reflection<Transform3DComponent>
{
    static constexpr const char* name         = "Transform3DComponent";
    static constexpr const char* display_name = "Transform 3D";

    template <typename Func>
    void attributes(Func&& func)
    {
        func(attribute_reflection<glm::vec3>{"position"});
        func(attribute_reflection<glm::quat>{"orientation"});
        func(attribute_reflection<glm::vec3>{"scale"});
    }
};

template <>
struct reflection<StaticModelComponent>
{
    static constexpr const char* name         = "StaticModelComponent";
    static constexpr const char* display_name = "Static Model";

    template <typename Func>
    void attributes(Func&& func)
    {
        func(attribute_reflection<std::string>{"mesh"});
        func(attribute_reflection<std::string>{"material"});
    }
};

template <>
struct reflection<AnimatedModelComponent>
{
    static constexpr const char* name         = "AnimatedModelComponent";
    static constexpr const char* display_name = "Animated Model";

    template <typename Func>
    void attributes(Func&& func)
    {
        func(attribute_reflection<std::string>{"mesh"});
        func(attribute_reflection<std::string>{"material"});
        func(attribute_reflection<std::string>{"animation_name"});
        func(attribute_reflection<float>{"animation_time"});
        func(attribute_reflection<float>{"animation_speed"});
    }
};

template <>
struct reflection<RigidBody3DComponent>
{
    static constexpr const char* name         = "RigidBody3DComponent";
    static constexpr const char* display_name = "Rigid Body 3D";

    template <typename Func>
    void attributes(Func&& func)
    {
        func(attribute_reflection<glm::vec3>{"velocity"});
        func(attribute_reflection<bool>{"gravity"});
        func(attribute_reflection<bool>{"frozen"});
        func(attribute_reflection<float>{"bounciness"});
        func(attribute_reflection<float>{"frictionCoefficient"});
        func(attribute_reflection<float>{"rollingResistance"});
        func(attribute_reflection<glm::vec3>{"force"});
        func(attribute_reflection<bool>{"onFloor"});
        func(attribute_reflection<std::shared_ptr<rigid_body_runtime>>{"runtime"});
    }
};

template <>
struct reflection<BoxCollider3DComponent>
{
    static constexpr const char* name         = "BoxCollider3DComponent";
    static constexpr const char* display_name = "Box Collider 3D";

    template <typename Func>
    void attributes(Func&& func)
    {
        func(attribute_reflection<glm::vec3>{"position"});
        func(attribute_reflection<glm::quat>{"orientation"});
        func(attribute_reflection<float>{"mass"});
        func(attribute_reflection<glm::vec3>{"halfExtents"});
        func(attribute_reflection<bool>{"applyScale"});
        func(attribute_reflection<std::shared_ptr<collider_runtime>>{"runtime"});
    }
};

template <>
struct reflection<SphereCollider3DComponent>
{
    static constexpr const char* name         = "SphereCollider3DComponent";
    static constexpr const char* display_name = "Sphere Collider 3D";

    template <typename Func>
    void attributes(Func&& func)
    {
        func(attribute_reflection<glm::vec3>{"position"});
        func(attribute_reflection<glm::quat>{"orientation"});
        func(attribute_reflection<float>{"mass"});
        func(attribute_reflection<float>{"radius"});
        func(attribute_reflection<std::shared_ptr<collider_runtime>>{"runtime"});
    }
};

template <>
struct reflection<CapsuleCollider3DComponent>
{
    static constexpr const char* name         = "CapsuleCollider3DComponent";
    static constexpr const char* display_name = "Capsule Collider 3D";

    template <typename Func>
    void attributes(Func&& func)
    {
        func(attribute_reflection<glm::vec3>{"position"});
        func(attribute_reflection<glm::quat>{"orientation"});
        func(attribute_reflection<float>{"mass"});
        func(attribute_reflection<float>{"radius"});
        func(attribute_reflection<float>{"height"});
        func(attribute_reflection<std::shared_ptr<collider_runtime>>{"runtime"});
    }
};

template <>
struct reflection<ScriptComponent>
{
    static constexpr const char* name         = "ScriptComponent";
    static constexpr const char* display_name = "Script";

    template <typename Func>
    void attributes(Func&& func)
    {
        func(attribute_reflection<std::string>{"script"});
        func(attribute_reflection<bool>{"active"});
        func(attribute_reflection<std::shared_ptr<lua::script>>{"script_runtime"});
    }
};

template <>
struct reflection<Camera3DComponent>
{
    static constexpr const char* name         = "Camera3DComponent";
    static constexpr const char* display_name = "Camera 3D";

    template <typename Func>
    void attributes(Func&& func)
    {
        func(attribute_reflection<glm::mat4>{"projection"});
        func(attribute_reflection<float>{"fov"});
        func(attribute_reflection<float>{"pitch"});
    }
};

template <>
struct reflection<PathComponent>
{
    static constexpr const char* name         = "PathComponent";
    static constexpr const char* display_name = "Path";

    template <typename Func>
    void attributes(Func&& func)
    {
        func(attribute_reflection<std::deque<glm::vec3>>{"markers"});
        func(attribute_reflection<float>{"speed"});
    }
};

template <>
struct reflection<LightComponent>
{
    static constexpr const char* name         = "LightComponent";
    static constexpr const char* display_name = "Light";

    template <typename Func>
    void attributes(Func&& func)
    {
        func(attribute_reflection<glm::vec3>{"colour"});
        func(attribute_reflection<float>{"brightness"});
    }
};

template <>
struct reflection<SunComponent>
{
    static constexpr const char* name         = "SunComponent";
    static constexpr const char* display_name = "Sun";

    template <typename Func>
    void attributes(Func&& func)
    {
        func(attribute_reflection<glm::vec3>{"colour"});
        func(attribute_reflection<float>{"brightness"});
        func(attribute_reflection<glm::vec3>{"direction"});
        func(attribute_reflection<bool>{"shadows"});
    }
};

template <>
struct reflection<AmbienceComponent>
{
    static constexpr const char* name         = "AmbienceComponent";
    static constexpr const char* display_name = "Ambience";

    template <typename Func>
    void attributes(Func&& func)
    {
        func(attribute_reflection<glm::vec3>{"colour"});
        func(attribute_reflection<float>{"brightness"});
    }
};

template <>
struct reflection<ParticleComponent>
{
    static constexpr const char* name         = "ParticleComponent";
    static constexpr const char* display_name = "Particle";

    template <typename Func>
    void attributes(Func&& func)
    {
        func(attribute_reflection<float>{"interval"});
        func(attribute_reflection<glm::vec3>{"velocity"});
        func(attribute_reflection<float>{"velocityNoise"});
        func(attribute_reflection<glm::vec3>{"acceleration"});
        func(attribute_reflection<glm::vec3>{"scale"});
        func(attribute_reflection<float>{"life"});
        func(attribute_reflection<float>{"accumulator"});
    }
};

template <>
struct reflection<CollisionEvent>
{
    static constexpr const char* name         = "CollisionEvent";
    static constexpr const char* display_name = "Collision Event";

    template <typename Func>
    void attributes(Func&& func)
    {
        func(attribute_reflection<spkt::entity>{"entity_a"});
        func(attribute_reflection<spkt::entity>{"entity_b"});
    }
};

template <>
struct reflection<PhysicsSingleton>
{
    static constexpr const char* name         = "PhysicsSingleton";
    static constexpr const char* display_name = "Physics Singleton";

    template <typename Func>
    void attributes(Func&& func)
    {
        func(attribute_reflection<std::shared_ptr<physics_runtime>>{"physics_runtime"});
    }
};

template <>
struct reflection<InputSingleton>
{
    static constexpr const char* name         = "InputSingleton";
    static constexpr const char* display_name = "Input Singleton";

    template <typename Func>
    void attributes(Func&& func)
    {
        func(attribute_reflection<std::shared_ptr<spkt::input_store>>{"input_store"});
    }
};

template <>
struct reflection<GameGridSingleton>
{
    static constexpr const char* name         = "GameGridSingleton";
    static constexpr const char* display_name = "Game Grid Singleton";

    template <typename Func>
    void attributes(Func&& func)
    {
        func(attribute_reflection<spkt::entity>{"hovered_square_entity"});
        func(attribute_reflection<spkt::entity>{"clicked_square_entity"});
        func(attribute_reflection<glm::ivec2>{"hovered_square"});
        func(attribute_reflection<std::optional<glm::ivec2>>{"clicked_square"});
    }
};

template <>
struct reflection<TileMapSingleton>
{
    static constexpr const char* name         = "TileMapSingleton";
    static constexpr const char* display_name = "Tile Map Singleton";

    template <typename Func>
    void attributes(Func&& func)
    {
        func(attribute_reflection<std::unordered_map<glm::ivec2, spkt::entity>>{"tiles"});
    }
};

template <>
struct reflection<CameraSingleton>
{
    static constexpr const char* name         = "CameraSingleton";
    static constexpr const char* display_name = "Camera Singleton";

    template <typename Func>
    void attributes(Func&& func)
    {
        func(attribute_reflection<spkt::entity>{"camera_entity"});
    }
};

template <>
struct reflection<ParticleSingleton>
{
    static constexpr const char* name         = "ParticleSingleton";
    static constexpr const char* display_name = "Particle Singleton";

    template <typename Func>
    void attributes(Func&& func)
    {
        func(attribute_reflection<std::shared_ptr<std::array<particle, NUM_PARTICLES>>>{"particles"});
        func(attribute_reflection<std::size_t>{"next_slot"});
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
