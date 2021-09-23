#pragma once
#include <Sprocket/Scene/ecs.h>

#include <string_view>
#include <string>

namespace spkt {

template <typename T, bool Savable, bool Scriptable>
struct reflattr
{
    const std::string_view                             name;
    const std::string_view                             display_name;
    T*                                                 value;
    const std::unordered_map<std::string, std::string> metadata;

    static constexpr bool  is_savable()    { return Savable; }
    static constexpr bool  is_scriptable() { return Scriptable; }
};

template <typename T>
struct reflcomp;

template <>
struct reflcomp<Runtime>
{
    static constexpr const char* name          = "Runtime";
    static constexpr const char* display_name  = "Runtime";

    static constexpr bool        is_savable()    { return false; }
    static constexpr bool        is_scriptable() { return false; }

    template <typename Func>
    void for_each_attribute(Runtime& component, Func&& func)
    {
    }

    template <typename Func>
    void for_each_attribute(const Runtime& component, Func&& func) const
    {
    }
};

template <>
struct reflcomp<Singleton>
{
    static constexpr const char* name          = "Singleton";
    static constexpr const char* display_name  = "Singleton";

    static constexpr bool        is_savable()    { return false; }
    static constexpr bool        is_scriptable() { return false; }

    template <typename Func>
    void for_each_attribute(Singleton& component, Func&& func)
    {
    }

    template <typename Func>
    void for_each_attribute(const Singleton& component, Func&& func) const
    {
    }
};

template <>
struct reflcomp<Event>
{
    static constexpr const char* name          = "Event";
    static constexpr const char* display_name  = "Event";

    static constexpr bool        is_savable()    { return false; }
    static constexpr bool        is_scriptable() { return false; }

    template <typename Func>
    void for_each_attribute(Event& component, Func&& func)
    {
    }

    template <typename Func>
    void for_each_attribute(const Event& component, Func&& func) const
    {
    }
};

template <>
struct reflcomp<NameComponent>
{
    static constexpr const char* name          = "NameComponent";
    static constexpr const char* display_name  = "Name";

    static constexpr bool        is_savable()    { return true; }
    static constexpr bool        is_scriptable() { return true; }

    template <typename Func>
    void for_each_attribute(NameComponent& component, Func&& func)
    {
        func(reflattr<std::string, true, true>{.name="name", .display_name="Name", .value=&component.name, .metadata={} });
    }

    template <typename Func>
    void for_each_attribute(const NameComponent& component, Func&& func) const
    {
        func(reflattr<const std::string, true, true>{.name="name", .display_name="Name", .value=&component.name, .metadata={} });
    }
};

template <>
struct reflcomp<Transform2DComponent>
{
    static constexpr const char* name          = "Transform2DComponent";
    static constexpr const char* display_name  = "Transform 2D";

    static constexpr bool        is_savable()    { return true; }
    static constexpr bool        is_scriptable() { return true; }

    template <typename Func>
    void for_each_attribute(Transform2DComponent& component, Func&& func)
    {
        func(reflattr<glm::vec2, true, true>{.name="position", .display_name="Position", .value=&component.position, .metadata={} });
        func(reflattr<float, true, true>{.name="rotation", .display_name="Rotation", .value=&component.rotation, .metadata={} });
        func(reflattr<glm::vec2, true, true>{.name="scale", .display_name="Scale", .value=&component.scale, .metadata={} });
    }

    template <typename Func>
    void for_each_attribute(const Transform2DComponent& component, Func&& func) const
    {
        func(reflattr<const glm::vec2, true, true>{.name="position", .display_name="Position", .value=&component.position, .metadata={} });
        func(reflattr<const float, true, true>{.name="rotation", .display_name="Rotation", .value=&component.rotation, .metadata={} });
        func(reflattr<const glm::vec2, true, true>{.name="scale", .display_name="Scale", .value=&component.scale, .metadata={} });
    }
};

template <>
struct reflcomp<Transform3DComponent>
{
    static constexpr const char* name          = "Transform3DComponent";
    static constexpr const char* display_name  = "Transform 3D";

    static constexpr bool        is_savable()    { return true; }
    static constexpr bool        is_scriptable() { return true; }

    template <typename Func>
    void for_each_attribute(Transform3DComponent& component, Func&& func)
    {
        func(reflattr<glm::vec3, true, true>{.name="position", .display_name="Position", .value=&component.position, .metadata={} });
        func(reflattr<glm::quat, true, false>{.name="orientation", .display_name="Orientation", .value=&component.orientation, .metadata={} });
        func(reflattr<glm::vec3, true, true>{.name="scale", .display_name="Scale", .value=&component.scale, .metadata={} });
    }

    template <typename Func>
    void for_each_attribute(const Transform3DComponent& component, Func&& func) const
    {
        func(reflattr<const glm::vec3, true, true>{.name="position", .display_name="Position", .value=&component.position, .metadata={} });
        func(reflattr<const glm::quat, true, false>{.name="orientation", .display_name="Orientation", .value=&component.orientation, .metadata={} });
        func(reflattr<const glm::vec3, true, true>{.name="scale", .display_name="Scale", .value=&component.scale, .metadata={} });
    }
};

template <>
struct reflcomp<StaticModelComponent>
{
    static constexpr const char* name          = "StaticModelComponent";
    static constexpr const char* display_name  = "Static Model";

    static constexpr bool        is_savable()    { return true; }
    static constexpr bool        is_scriptable() { return true; }

    template <typename Func>
    void for_each_attribute(StaticModelComponent& component, Func&& func)
    {
        func(reflattr<std::string, true, true>{.name="mesh", .display_name="Mesh", .value=&component.mesh, .metadata={{ "file_filter", "*.obj" }} });
        func(reflattr<std::string, true, true>{.name="material", .display_name="Material", .value=&component.material, .metadata={{ "file_filter", "*.obj" }} });
    }

    template <typename Func>
    void for_each_attribute(const StaticModelComponent& component, Func&& func) const
    {
        func(reflattr<const std::string, true, true>{.name="mesh", .display_name="Mesh", .value=&component.mesh, .metadata={{ "file_filter", "*.obj" }} });
        func(reflattr<const std::string, true, true>{.name="material", .display_name="Material", .value=&component.material, .metadata={{ "file_filter", "*.obj" }} });
    }
};

template <>
struct reflcomp<AnimatedModelComponent>
{
    static constexpr const char* name          = "AnimatedModelComponent";
    static constexpr const char* display_name  = "Animated Model";

    static constexpr bool        is_savable()    { return true; }
    static constexpr bool        is_scriptable() { return true; }

    template <typename Func>
    void for_each_attribute(AnimatedModelComponent& component, Func&& func)
    {
        func(reflattr<std::string, true, true>{.name="mesh", .display_name="Mesh", .value=&component.mesh, .metadata={{ "file_filter", "*.obj" }} });
        func(reflattr<std::string, true, true>{.name="material", .display_name="Material", .value=&component.material, .metadata={{ "file_filter", "*.obj" }} });
        func(reflattr<std::string, true, true>{.name="animation_name", .display_name="Animation name", .value=&component.animation_name, .metadata={} });
        func(reflattr<float, true, true>{.name="animation_time", .display_name="Animation Time", .value=&component.animation_time, .metadata={} });
        func(reflattr<float, true, true>{.name="animation_speed", .display_name="Animation Speed", .value=&component.animation_speed, .metadata={} });
    }

    template <typename Func>
    void for_each_attribute(const AnimatedModelComponent& component, Func&& func) const
    {
        func(reflattr<const std::string, true, true>{.name="mesh", .display_name="Mesh", .value=&component.mesh, .metadata={{ "file_filter", "*.obj" }} });
        func(reflattr<const std::string, true, true>{.name="material", .display_name="Material", .value=&component.material, .metadata={{ "file_filter", "*.obj" }} });
        func(reflattr<const std::string, true, true>{.name="animation_name", .display_name="Animation name", .value=&component.animation_name, .metadata={} });
        func(reflattr<const float, true, true>{.name="animation_time", .display_name="Animation Time", .value=&component.animation_time, .metadata={} });
        func(reflattr<const float, true, true>{.name="animation_speed", .display_name="Animation Speed", .value=&component.animation_speed, .metadata={} });
    }
};

template <>
struct reflcomp<RigidBody3DComponent>
{
    static constexpr const char* name          = "RigidBody3DComponent";
    static constexpr const char* display_name  = "Rigid Body 3D";

    static constexpr bool        is_savable()    { return true; }
    static constexpr bool        is_scriptable() { return true; }

    template <typename Func>
    void for_each_attribute(RigidBody3DComponent& component, Func&& func)
    {
        func(reflattr<glm::vec3, true, true>{.name="velocity", .display_name="Velocity", .value=&component.velocity, .metadata={} });
        func(reflattr<bool, true, true>{.name="gravity", .display_name="Gravity", .value=&component.gravity, .metadata={} });
        func(reflattr<bool, true, true>{.name="frozen", .display_name="Frozen", .value=&component.frozen, .metadata={} });
        func(reflattr<float, true, true>{.name="bounciness", .display_name="Bounciness", .value=&component.bounciness, .metadata={{ "lower_limit", "0.0" }, { "upper_limit", "1.0" }} });
        func(reflattr<float, true, true>{.name="frictionCoefficient", .display_name="Friction Coefficient", .value=&component.frictionCoefficient, .metadata={{ "lower_limit", "0.0" }, { "upper_limit", "1.0" }} });
        func(reflattr<float, true, true>{.name="rollingResistance", .display_name="Rolling Resistance", .value=&component.rollingResistance, .metadata={{ "lower_limit", "0.0" }, { "upper_limit", "1.0" }} });
        func(reflattr<glm::vec3, false, true>{.name="force", .display_name="Force", .value=&component.force, .metadata={} });
        func(reflattr<bool, false, true>{.name="onFloor", .display_name="OnFloor", .value=&component.onFloor, .metadata={} });
        func(reflattr<std::shared_ptr<rigid_body_runtime>, false, false>{.name="runtime", .display_name="Runtime", .value=&component.runtime, .metadata={} });
    }

    template <typename Func>
    void for_each_attribute(const RigidBody3DComponent& component, Func&& func) const
    {
        func(reflattr<const glm::vec3, true, true>{.name="velocity", .display_name="Velocity", .value=&component.velocity, .metadata={} });
        func(reflattr<const bool, true, true>{.name="gravity", .display_name="Gravity", .value=&component.gravity, .metadata={} });
        func(reflattr<const bool, true, true>{.name="frozen", .display_name="Frozen", .value=&component.frozen, .metadata={} });
        func(reflattr<const float, true, true>{.name="bounciness", .display_name="Bounciness", .value=&component.bounciness, .metadata={{ "lower_limit", "0.0" }, { "upper_limit", "1.0" }} });
        func(reflattr<const float, true, true>{.name="frictionCoefficient", .display_name="Friction Coefficient", .value=&component.frictionCoefficient, .metadata={{ "lower_limit", "0.0" }, { "upper_limit", "1.0" }} });
        func(reflattr<const float, true, true>{.name="rollingResistance", .display_name="Rolling Resistance", .value=&component.rollingResistance, .metadata={{ "lower_limit", "0.0" }, { "upper_limit", "1.0" }} });
        func(reflattr<const glm::vec3, false, true>{.name="force", .display_name="Force", .value=&component.force, .metadata={} });
        func(reflattr<const bool, false, true>{.name="onFloor", .display_name="OnFloor", .value=&component.onFloor, .metadata={} });
        func(reflattr<const std::shared_ptr<rigid_body_runtime>, false, false>{.name="runtime", .display_name="Runtime", .value=&component.runtime, .metadata={} });
    }
};

template <>
struct reflcomp<BoxCollider3DComponent>
{
    static constexpr const char* name          = "BoxCollider3DComponent";
    static constexpr const char* display_name  = "Box Collider 3D";

    static constexpr bool        is_savable()    { return true; }
    static constexpr bool        is_scriptable() { return true; }

    template <typename Func>
    void for_each_attribute(BoxCollider3DComponent& component, Func&& func)
    {
        func(reflattr<glm::vec3, true, true>{.name="position", .display_name="Position", .value=&component.position, .metadata={} });
        func(reflattr<glm::quat, true, false>{.name="orientation", .display_name="Orientation", .value=&component.orientation, .metadata={} });
        func(reflattr<float, true, true>{.name="mass", .display_name="Mass", .value=&component.mass, .metadata={} });
        func(reflattr<glm::vec3, true, true>{.name="halfExtents", .display_name="Half Extents", .value=&component.halfExtents, .metadata={} });
        func(reflattr<bool, true, true>{.name="applyScale", .display_name="Apply Scale", .value=&component.applyScale, .metadata={} });
        func(reflattr<std::shared_ptr<collider_runtime>, false, false>{.name="runtime", .display_name="Runtime", .value=&component.runtime, .metadata={} });
    }

    template <typename Func>
    void for_each_attribute(const BoxCollider3DComponent& component, Func&& func) const
    {
        func(reflattr<const glm::vec3, true, true>{.name="position", .display_name="Position", .value=&component.position, .metadata={} });
        func(reflattr<const glm::quat, true, false>{.name="orientation", .display_name="Orientation", .value=&component.orientation, .metadata={} });
        func(reflattr<const float, true, true>{.name="mass", .display_name="Mass", .value=&component.mass, .metadata={} });
        func(reflattr<const glm::vec3, true, true>{.name="halfExtents", .display_name="Half Extents", .value=&component.halfExtents, .metadata={} });
        func(reflattr<const bool, true, true>{.name="applyScale", .display_name="Apply Scale", .value=&component.applyScale, .metadata={} });
        func(reflattr<const std::shared_ptr<collider_runtime>, false, false>{.name="runtime", .display_name="Runtime", .value=&component.runtime, .metadata={} });
    }
};

template <>
struct reflcomp<SphereCollider3DComponent>
{
    static constexpr const char* name          = "SphereCollider3DComponent";
    static constexpr const char* display_name  = "Sphere Collider 3D";

    static constexpr bool        is_savable()    { return true; }
    static constexpr bool        is_scriptable() { return true; }

    template <typename Func>
    void for_each_attribute(SphereCollider3DComponent& component, Func&& func)
    {
        func(reflattr<glm::vec3, true, true>{.name="position", .display_name="Position", .value=&component.position, .metadata={} });
        func(reflattr<glm::quat, true, false>{.name="orientation", .display_name="Orientation", .value=&component.orientation, .metadata={} });
        func(reflattr<float, true, true>{.name="mass", .display_name="Mass", .value=&component.mass, .metadata={} });
        func(reflattr<float, true, true>{.name="radius", .display_name="Radius", .value=&component.radius, .metadata={} });
        func(reflattr<std::shared_ptr<collider_runtime>, false, false>{.name="runtime", .display_name="Runtime", .value=&component.runtime, .metadata={} });
    }

    template <typename Func>
    void for_each_attribute(const SphereCollider3DComponent& component, Func&& func) const
    {
        func(reflattr<const glm::vec3, true, true>{.name="position", .display_name="Position", .value=&component.position, .metadata={} });
        func(reflattr<const glm::quat, true, false>{.name="orientation", .display_name="Orientation", .value=&component.orientation, .metadata={} });
        func(reflattr<const float, true, true>{.name="mass", .display_name="Mass", .value=&component.mass, .metadata={} });
        func(reflattr<const float, true, true>{.name="radius", .display_name="Radius", .value=&component.radius, .metadata={} });
        func(reflattr<const std::shared_ptr<collider_runtime>, false, false>{.name="runtime", .display_name="Runtime", .value=&component.runtime, .metadata={} });
    }
};

template <>
struct reflcomp<CapsuleCollider3DComponent>
{
    static constexpr const char* name          = "CapsuleCollider3DComponent";
    static constexpr const char* display_name  = "Capsule Collider 3D";

    static constexpr bool        is_savable()    { return true; }
    static constexpr bool        is_scriptable() { return true; }

    template <typename Func>
    void for_each_attribute(CapsuleCollider3DComponent& component, Func&& func)
    {
        func(reflattr<glm::vec3, true, true>{.name="position", .display_name="Position", .value=&component.position, .metadata={} });
        func(reflattr<glm::quat, true, false>{.name="orientation", .display_name="Orientation", .value=&component.orientation, .metadata={} });
        func(reflattr<float, true, true>{.name="mass", .display_name="Mass", .value=&component.mass, .metadata={} });
        func(reflattr<float, true, true>{.name="radius", .display_name="Radius", .value=&component.radius, .metadata={} });
        func(reflattr<float, true, true>{.name="height", .display_name="Height", .value=&component.height, .metadata={} });
        func(reflattr<std::shared_ptr<collider_runtime>, false, false>{.name="runtime", .display_name="Runtime", .value=&component.runtime, .metadata={} });
    }

    template <typename Func>
    void for_each_attribute(const CapsuleCollider3DComponent& component, Func&& func) const
    {
        func(reflattr<const glm::vec3, true, true>{.name="position", .display_name="Position", .value=&component.position, .metadata={} });
        func(reflattr<const glm::quat, true, false>{.name="orientation", .display_name="Orientation", .value=&component.orientation, .metadata={} });
        func(reflattr<const float, true, true>{.name="mass", .display_name="Mass", .value=&component.mass, .metadata={} });
        func(reflattr<const float, true, true>{.name="radius", .display_name="Radius", .value=&component.radius, .metadata={} });
        func(reflattr<const float, true, true>{.name="height", .display_name="Height", .value=&component.height, .metadata={} });
        func(reflattr<const std::shared_ptr<collider_runtime>, false, false>{.name="runtime", .display_name="Runtime", .value=&component.runtime, .metadata={} });
    }
};

template <>
struct reflcomp<ScriptComponent>
{
    static constexpr const char* name          = "ScriptComponent";
    static constexpr const char* display_name  = "Script";

    static constexpr bool        is_savable()    { return true; }
    static constexpr bool        is_scriptable() { return true; }

    template <typename Func>
    void for_each_attribute(ScriptComponent& component, Func&& func)
    {
        func(reflattr<std::string, true, true>{.name="script", .display_name="Script", .value=&component.script, .metadata={{ "file_filter", "*.obj" }} });
        func(reflattr<bool, true, true>{.name="active", .display_name="Active", .value=&component.active, .metadata={} });
        func(reflattr<std::shared_ptr<lua::script>, false, false>{.name="script_runtime", .display_name="Script Runtime", .value=&component.script_runtime, .metadata={} });
    }

    template <typename Func>
    void for_each_attribute(const ScriptComponent& component, Func&& func) const
    {
        func(reflattr<const std::string, true, true>{.name="script", .display_name="Script", .value=&component.script, .metadata={{ "file_filter", "*.obj" }} });
        func(reflattr<const bool, true, true>{.name="active", .display_name="Active", .value=&component.active, .metadata={} });
        func(reflattr<const std::shared_ptr<lua::script>, false, false>{.name="script_runtime", .display_name="Script Runtime", .value=&component.script_runtime, .metadata={} });
    }
};

template <>
struct reflcomp<Camera3DComponent>
{
    static constexpr const char* name          = "Camera3DComponent";
    static constexpr const char* display_name  = "Camera 3D";

    static constexpr bool        is_savable()    { return true; }
    static constexpr bool        is_scriptable() { return true; }

    template <typename Func>
    void for_each_attribute(Camera3DComponent& component, Func&& func)
    {
        func(reflattr<glm::mat4, false, false>{.name="projection", .display_name="Projection", .value=&component.projection, .metadata={} });
        func(reflattr<float, true, true>{.name="fov", .display_name="FOV", .value=&component.fov, .metadata={} });
        func(reflattr<float, true, true>{.name="pitch", .display_name="Pitch", .value=&component.pitch, .metadata={} });
    }

    template <typename Func>
    void for_each_attribute(const Camera3DComponent& component, Func&& func) const
    {
        func(reflattr<const glm::mat4, false, false>{.name="projection", .display_name="Projection", .value=&component.projection, .metadata={} });
        func(reflattr<const float, true, true>{.name="fov", .display_name="FOV", .value=&component.fov, .metadata={} });
        func(reflattr<const float, true, true>{.name="pitch", .display_name="Pitch", .value=&component.pitch, .metadata={} });
    }
};

template <>
struct reflcomp<PathComponent>
{
    static constexpr const char* name          = "PathComponent";
    static constexpr const char* display_name  = "Path";

    static constexpr bool        is_savable()    { return true; }
    static constexpr bool        is_scriptable() { return true; }

    template <typename Func>
    void for_each_attribute(PathComponent& component, Func&& func)
    {
        func(reflattr<std::deque<glm::vec3>, false, false>{.name="markers", .display_name="Markers", .value=&component.markers, .metadata={} });
        func(reflattr<float, true, true>{.name="speed", .display_name="Speed", .value=&component.speed, .metadata={} });
    }

    template <typename Func>
    void for_each_attribute(const PathComponent& component, Func&& func) const
    {
        func(reflattr<const std::deque<glm::vec3>, false, false>{.name="markers", .display_name="Markers", .value=&component.markers, .metadata={} });
        func(reflattr<const float, true, true>{.name="speed", .display_name="Speed", .value=&component.speed, .metadata={} });
    }
};

template <>
struct reflcomp<LightComponent>
{
    static constexpr const char* name          = "LightComponent";
    static constexpr const char* display_name  = "Light";

    static constexpr bool        is_savable()    { return true; }
    static constexpr bool        is_scriptable() { return true; }

    template <typename Func>
    void for_each_attribute(LightComponent& component, Func&& func)
    {
        func(reflattr<glm::vec3, true, true>{.name="colour", .display_name="Colour", .value=&component.colour, .metadata={{ "colour", "" }} });
        func(reflattr<float, true, true>{.name="brightness", .display_name="Brightness", .value=&component.brightness, .metadata={} });
    }

    template <typename Func>
    void for_each_attribute(const LightComponent& component, Func&& func) const
    {
        func(reflattr<const glm::vec3, true, true>{.name="colour", .display_name="Colour", .value=&component.colour, .metadata={{ "colour", "" }} });
        func(reflattr<const float, true, true>{.name="brightness", .display_name="Brightness", .value=&component.brightness, .metadata={} });
    }
};

template <>
struct reflcomp<SunComponent>
{
    static constexpr const char* name          = "SunComponent";
    static constexpr const char* display_name  = "Sun";

    static constexpr bool        is_savable()    { return true; }
    static constexpr bool        is_scriptable() { return true; }

    template <typename Func>
    void for_each_attribute(SunComponent& component, Func&& func)
    {
        func(reflattr<glm::vec3, true, true>{.name="colour", .display_name="Colour", .value=&component.colour, .metadata={{ "colour", "" }} });
        func(reflattr<float, true, true>{.name="brightness", .display_name="Brightness", .value=&component.brightness, .metadata={} });
        func(reflattr<glm::vec3, true, true>{.name="direction", .display_name="Direction", .value=&component.direction, .metadata={} });
        func(reflattr<bool, true, true>{.name="shadows", .display_name="Shadows", .value=&component.shadows, .metadata={} });
    }

    template <typename Func>
    void for_each_attribute(const SunComponent& component, Func&& func) const
    {
        func(reflattr<const glm::vec3, true, true>{.name="colour", .display_name="Colour", .value=&component.colour, .metadata={{ "colour", "" }} });
        func(reflattr<const float, true, true>{.name="brightness", .display_name="Brightness", .value=&component.brightness, .metadata={} });
        func(reflattr<const glm::vec3, true, true>{.name="direction", .display_name="Direction", .value=&component.direction, .metadata={} });
        func(reflattr<const bool, true, true>{.name="shadows", .display_name="Shadows", .value=&component.shadows, .metadata={} });
    }
};

template <>
struct reflcomp<AmbienceComponent>
{
    static constexpr const char* name          = "AmbienceComponent";
    static constexpr const char* display_name  = "Ambience";

    static constexpr bool        is_savable()    { return true; }
    static constexpr bool        is_scriptable() { return true; }

    template <typename Func>
    void for_each_attribute(AmbienceComponent& component, Func&& func)
    {
        func(reflattr<glm::vec3, true, true>{.name="colour", .display_name="Colour", .value=&component.colour, .metadata={{ "colour", "" }} });
        func(reflattr<float, true, true>{.name="brightness", .display_name="Brightness", .value=&component.brightness, .metadata={} });
    }

    template <typename Func>
    void for_each_attribute(const AmbienceComponent& component, Func&& func) const
    {
        func(reflattr<const glm::vec3, true, true>{.name="colour", .display_name="Colour", .value=&component.colour, .metadata={{ "colour", "" }} });
        func(reflattr<const float, true, true>{.name="brightness", .display_name="Brightness", .value=&component.brightness, .metadata={} });
    }
};

template <>
struct reflcomp<ParticleComponent>
{
    static constexpr const char* name          = "ParticleComponent";
    static constexpr const char* display_name  = "Particle";

    static constexpr bool        is_savable()    { return true; }
    static constexpr bool        is_scriptable() { return true; }

    template <typename Func>
    void for_each_attribute(ParticleComponent& component, Func&& func)
    {
        func(reflattr<float, true, true>{.name="interval", .display_name="Interval", .value=&component.interval, .metadata={} });
        func(reflattr<glm::vec3, true, true>{.name="velocity", .display_name="Velocity", .value=&component.velocity, .metadata={} });
        func(reflattr<float, true, true>{.name="velocityNoise", .display_name="Velocity Noise", .value=&component.velocityNoise, .metadata={} });
        func(reflattr<glm::vec3, true, true>{.name="acceleration", .display_name="Acceleration", .value=&component.acceleration, .metadata={} });
        func(reflattr<glm::vec3, true, true>{.name="scale", .display_name="Scale", .value=&component.scale, .metadata={} });
        func(reflattr<float, true, true>{.name="life", .display_name="Life", .value=&component.life, .metadata={} });
        func(reflattr<float, false, false>{.name="accumulator", .display_name="Accumulator", .value=&component.accumulator, .metadata={} });
    }

    template <typename Func>
    void for_each_attribute(const ParticleComponent& component, Func&& func) const
    {
        func(reflattr<const float, true, true>{.name="interval", .display_name="Interval", .value=&component.interval, .metadata={} });
        func(reflattr<const glm::vec3, true, true>{.name="velocity", .display_name="Velocity", .value=&component.velocity, .metadata={} });
        func(reflattr<const float, true, true>{.name="velocityNoise", .display_name="Velocity Noise", .value=&component.velocityNoise, .metadata={} });
        func(reflattr<const glm::vec3, true, true>{.name="acceleration", .display_name="Acceleration", .value=&component.acceleration, .metadata={} });
        func(reflattr<const glm::vec3, true, true>{.name="scale", .display_name="Scale", .value=&component.scale, .metadata={} });
        func(reflattr<const float, true, true>{.name="life", .display_name="Life", .value=&component.life, .metadata={} });
        func(reflattr<const float, false, false>{.name="accumulator", .display_name="Accumulator", .value=&component.accumulator, .metadata={} });
    }
};

template <>
struct reflcomp<CollisionEvent>
{
    static constexpr const char* name          = "CollisionEvent";
    static constexpr const char* display_name  = "Collision Event";

    static constexpr bool        is_savable()    { return false; }
    static constexpr bool        is_scriptable() { return true; }

    template <typename Func>
    void for_each_attribute(CollisionEvent& component, Func&& func)
    {
        func(reflattr<spkt::entity, true, true>{.name="entity_a", .display_name="Entity A", .value=&component.entity_a, .metadata={} });
        func(reflattr<spkt::entity, true, true>{.name="entity_b", .display_name="Entity B", .value=&component.entity_b, .metadata={} });
    }

    template <typename Func>
    void for_each_attribute(const CollisionEvent& component, Func&& func) const
    {
        func(reflattr<const spkt::entity, true, true>{.name="entity_a", .display_name="Entity A", .value=&component.entity_a, .metadata={} });
        func(reflattr<const spkt::entity, true, true>{.name="entity_b", .display_name="Entity B", .value=&component.entity_b, .metadata={} });
    }
};

template <>
struct reflcomp<PhysicsSingleton>
{
    static constexpr const char* name          = "PhysicsSingleton";
    static constexpr const char* display_name  = "Physics Singleton";

    static constexpr bool        is_savable()    { return false; }
    static constexpr bool        is_scriptable() { return false; }

    template <typename Func>
    void for_each_attribute(PhysicsSingleton& component, Func&& func)
    {
        func(reflattr<std::shared_ptr<physics_runtime>, false, false>{.name="physics_runtime", .display_name="Physics Runtime", .value=&component.physics_runtime, .metadata={} });
    }

    template <typename Func>
    void for_each_attribute(const PhysicsSingleton& component, Func&& func) const
    {
        func(reflattr<const std::shared_ptr<physics_runtime>, false, false>{.name="physics_runtime", .display_name="Physics Runtime", .value=&component.physics_runtime, .metadata={} });
    }
};

template <>
struct reflcomp<InputSingleton>
{
    static constexpr const char* name          = "InputSingleton";
    static constexpr const char* display_name  = "Input Singleton";

    static constexpr bool        is_savable()    { return false; }
    static constexpr bool        is_scriptable() { return false; }

    template <typename Func>
    void for_each_attribute(InputSingleton& component, Func&& func)
    {
        func(reflattr<std::shared_ptr<spkt::input_store>, true, true>{.name="input_store", .display_name="Input Store", .value=&component.input_store, .metadata={} });
    }

    template <typename Func>
    void for_each_attribute(const InputSingleton& component, Func&& func) const
    {
        func(reflattr<const std::shared_ptr<spkt::input_store>, true, true>{.name="input_store", .display_name="Input Store", .value=&component.input_store, .metadata={} });
    }
};

template <>
struct reflcomp<GameGridSingleton>
{
    static constexpr const char* name          = "GameGridSingleton";
    static constexpr const char* display_name  = "Game Grid Singleton";

    static constexpr bool        is_savable()    { return false; }
    static constexpr bool        is_scriptable() { return false; }

    template <typename Func>
    void for_each_attribute(GameGridSingleton& component, Func&& func)
    {
        func(reflattr<spkt::entity, true, true>{.name="hovered_square_entity", .display_name="Hovered Square Entity", .value=&component.hovered_square_entity, .metadata={} });
        func(reflattr<spkt::entity, true, true>{.name="clicked_square_entity", .display_name="Clicked Square Entity", .value=&component.clicked_square_entity, .metadata={} });
        func(reflattr<glm::ivec2, true, true>{.name="hovered_square", .display_name="Hovered Square", .value=&component.hovered_square, .metadata={} });
        func(reflattr<std::optional<glm::ivec2>, true, true>{.name="clicked_square", .display_name="Clicked Square", .value=&component.clicked_square, .metadata={} });
    }

    template <typename Func>
    void for_each_attribute(const GameGridSingleton& component, Func&& func) const
    {
        func(reflattr<const spkt::entity, true, true>{.name="hovered_square_entity", .display_name="Hovered Square Entity", .value=&component.hovered_square_entity, .metadata={} });
        func(reflattr<const spkt::entity, true, true>{.name="clicked_square_entity", .display_name="Clicked Square Entity", .value=&component.clicked_square_entity, .metadata={} });
        func(reflattr<const glm::ivec2, true, true>{.name="hovered_square", .display_name="Hovered Square", .value=&component.hovered_square, .metadata={} });
        func(reflattr<const std::optional<glm::ivec2>, true, true>{.name="clicked_square", .display_name="Clicked Square", .value=&component.clicked_square, .metadata={} });
    }
};

template <>
struct reflcomp<TileMapSingleton>
{
    static constexpr const char* name          = "TileMapSingleton";
    static constexpr const char* display_name  = "Tile Map Singleton";

    static constexpr bool        is_savable()    { return true; }
    static constexpr bool        is_scriptable() { return false; }

    template <typename Func>
    void for_each_attribute(TileMapSingleton& component, Func&& func)
    {
        func(reflattr<std::unordered_map<glm::ivec2, spkt::entity>, true, true>{.name="tiles", .display_name="Tiles", .value=&component.tiles, .metadata={} });
    }

    template <typename Func>
    void for_each_attribute(const TileMapSingleton& component, Func&& func) const
    {
        func(reflattr<const std::unordered_map<glm::ivec2, spkt::entity>, true, true>{.name="tiles", .display_name="Tiles", .value=&component.tiles, .metadata={} });
    }
};

template <>
struct reflcomp<CameraSingleton>
{
    static constexpr const char* name          = "CameraSingleton";
    static constexpr const char* display_name  = "Camera Singleton";

    static constexpr bool        is_savable()    { return false; }
    static constexpr bool        is_scriptable() { return false; }

    template <typename Func>
    void for_each_attribute(CameraSingleton& component, Func&& func)
    {
        func(reflattr<spkt::entity, true, true>{.name="camera_entity", .display_name="Camera Entity", .value=&component.camera_entity, .metadata={} });
    }

    template <typename Func>
    void for_each_attribute(const CameraSingleton& component, Func&& func) const
    {
        func(reflattr<const spkt::entity, true, true>{.name="camera_entity", .display_name="Camera Entity", .value=&component.camera_entity, .metadata={} });
    }
};

template <>
struct reflcomp<ParticleSingleton>
{
    static constexpr const char* name          = "ParticleSingleton";
    static constexpr const char* display_name  = "Particle Singleton";

    static constexpr bool        is_savable()    { return false; }
    static constexpr bool        is_scriptable() { return false; }

    template <typename Func>
    void for_each_attribute(ParticleSingleton& component, Func&& func)
    {
        func(reflattr<std::shared_ptr<std::array<particle, NUM_PARTICLES>>, true, true>{.name="particles", .display_name="Particles", .value=&component.particles, .metadata={} });
        func(reflattr<std::size_t, true, true>{.name="next_slot", .display_name="Next Slot", .value=&component.next_slot, .metadata={} });
    }

    template <typename Func>
    void for_each_attribute(const ParticleSingleton& component, Func&& func) const
    {
        func(reflattr<const std::shared_ptr<std::array<particle, NUM_PARTICLES>>, true, true>{.name="particles", .display_name="Particles", .value=&component.particles, .metadata={} });
        func(reflattr<const std::size_t, true, true>{.name="next_slot", .display_name="Next Slot", .value=&component.next_slot, .metadata={} });
    }
};


template <typename Func>
void for_each_component(Func&& func)
{
    func(reflcomp<Runtime>{});
    func(reflcomp<Singleton>{});
    func(reflcomp<Event>{});
    func(reflcomp<NameComponent>{});
    func(reflcomp<Transform2DComponent>{});
    func(reflcomp<Transform3DComponent>{});
    func(reflcomp<StaticModelComponent>{});
    func(reflcomp<AnimatedModelComponent>{});
    func(reflcomp<RigidBody3DComponent>{});
    func(reflcomp<BoxCollider3DComponent>{});
    func(reflcomp<SphereCollider3DComponent>{});
    func(reflcomp<CapsuleCollider3DComponent>{});
    func(reflcomp<ScriptComponent>{});
    func(reflcomp<Camera3DComponent>{});
    func(reflcomp<PathComponent>{});
    func(reflcomp<LightComponent>{});
    func(reflcomp<SunComponent>{});
    func(reflcomp<AmbienceComponent>{});
    func(reflcomp<ParticleComponent>{});
    func(reflcomp<CollisionEvent>{});
    func(reflcomp<PhysicsSingleton>{});
    func(reflcomp<InputSingleton>{});
    func(reflcomp<GameGridSingleton>{});
    func(reflcomp<TileMapSingleton>{});
    func(reflcomp<CameraSingleton>{});
    func(reflcomp<ParticleSingleton>{});
}

}
