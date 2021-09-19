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
    static constexpr std::string_view component_name = "Runtime";

    template <typename Func>
    void attributes(Func&& func)
    {
    }
};

template <>
struct reflection<Singleton>
{
    static constexpr std::string_view component_name = "Singleton";

    template <typename Func>
    void attributes(Func&& func)
    {
    }
};

template <>
struct reflection<Event>
{
    static constexpr std::string_view component_name = "Event";

    template <typename Func>
    void attributes(Func&& func)
    {
    }
};

template <>
struct reflection<NameComponent>
{
    static constexpr std::string_view component_name = "NameComponent";

    template <typename Func>
    void attributes(Func&& func)
    {
        func(attribute_reflection<std::string>{"name"});
    }
};

template <>
struct reflection<Transform2DComponent>
{
    static constexpr std::string_view component_name = "Transform2DComponent";

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
    static constexpr std::string_view component_name = "Transform3DComponent";

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
    static constexpr std::string_view component_name = "StaticModelComponent";

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
    static constexpr std::string_view component_name = "AnimatedModelComponent";

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
    static constexpr std::string_view component_name = "RigidBody3DComponent";

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
    static constexpr std::string_view component_name = "BoxCollider3DComponent";

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
    static constexpr std::string_view component_name = "SphereCollider3DComponent";

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
    static constexpr std::string_view component_name = "CapsuleCollider3DComponent";

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
    static constexpr std::string_view component_name = "ScriptComponent";

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
    static constexpr std::string_view component_name = "Camera3DComponent";

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
    static constexpr std::string_view component_name = "PathComponent";

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
    static constexpr std::string_view component_name = "LightComponent";

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
    static constexpr std::string_view component_name = "SunComponent";

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
    static constexpr std::string_view component_name = "AmbienceComponent";

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
    static constexpr std::string_view component_name = "ParticleComponent";

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
    static constexpr std::string_view component_name = "CollisionEvent";

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
    static constexpr std::string_view component_name = "PhysicsSingleton";

    template <typename Func>
    void attributes(Func&& func)
    {
        func(attribute_reflection<std::shared_ptr<physics_runtime>>{"physics_runtime"});
    }
};

template <>
struct reflection<InputSingleton>
{
    static constexpr std::string_view component_name = "InputSingleton";

    template <typename Func>
    void attributes(Func&& func)
    {
        func(attribute_reflection<std::shared_ptr<spkt::input_store>>{"input_store"});
    }
};

template <>
struct reflection<GameGridSingleton>
{
    static constexpr std::string_view component_name = "GameGridSingleton";

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
    static constexpr std::string_view component_name = "TileMapSingleton";

    template <typename Func>
    void attributes(Func&& func)
    {
        func(attribute_reflection<std::unordered_map<glm::ivec2, spkt::entity>>{"tiles"});
    }
};

template <>
struct reflection<CameraSingleton>
{
    static constexpr std::string_view component_name = "CameraSingleton";

    template <typename Func>
    void attributes(Func&& func)
    {
        func(attribute_reflection<spkt::entity>{"camera_entity"});
    }
};

template <>
struct reflection<ParticleSingleton>
{
    static constexpr std::string_view component_name = "ParticleSingleton";

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
