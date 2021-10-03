#pragma once
#include "ecs.h"

#include <string_view>
#include <string>

namespace game {

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
        func(reflattr<std::shared_ptr<spkt::lua::script>, false, false>{.name="script_runtime", .display_name="Script Runtime", .value=&component.script_runtime, .metadata={} });
    }

    template <typename Func>
    void for_each_attribute(const ScriptComponent& component, Func&& func) const
    {
        func(reflattr<const std::string, true, true>{.name="script", .display_name="Script", .value=&component.script, .metadata={{ "file_filter", "*.obj" }} });
        func(reflattr<const bool, true, true>{.name="active", .display_name="Active", .value=&component.active, .metadata={} });
        func(reflattr<const std::shared_ptr<spkt::lua::script>, false, false>{.name="script_runtime", .display_name="Script Runtime", .value=&component.script_runtime, .metadata={} });
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
        func(reflattr<float, true, true>{.name="fov", .display_name="FOV", .value=&component.fov, .metadata={} });
        func(reflattr<float, true, true>{.name="pitch", .display_name="Pitch", .value=&component.pitch, .metadata={} });
    }

    template <typename Func>
    void for_each_attribute(const Camera3DComponent& component, Func&& func) const
    {
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
        func(reflattr<glm::vec3, true, true>{.name="colour", .display_name="Colour", .value=&component.colour, .metadata={{ "is_colour", "" }} });
        func(reflattr<float, true, true>{.name="brightness", .display_name="Brightness", .value=&component.brightness, .metadata={} });
    }

    template <typename Func>
    void for_each_attribute(const LightComponent& component, Func&& func) const
    {
        func(reflattr<const glm::vec3, true, true>{.name="colour", .display_name="Colour", .value=&component.colour, .metadata={{ "is_colour", "" }} });
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
        func(reflattr<glm::vec3, true, true>{.name="colour", .display_name="Colour", .value=&component.colour, .metadata={{ "is_colour", "" }} });
        func(reflattr<float, true, true>{.name="brightness", .display_name="Brightness", .value=&component.brightness, .metadata={} });
        func(reflattr<glm::vec3, true, true>{.name="direction", .display_name="Direction", .value=&component.direction, .metadata={} });
        func(reflattr<bool, true, true>{.name="shadows", .display_name="Shadows", .value=&component.shadows, .metadata={} });
    }

    template <typename Func>
    void for_each_attribute(const SunComponent& component, Func&& func) const
    {
        func(reflattr<const glm::vec3, true, true>{.name="colour", .display_name="Colour", .value=&component.colour, .metadata={{ "is_colour", "" }} });
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
        func(reflattr<glm::vec3, true, true>{.name="colour", .display_name="Colour", .value=&component.colour, .metadata={{ "is_colour", "" }} });
        func(reflattr<float, true, true>{.name="brightness", .display_name="Brightness", .value=&component.brightness, .metadata={} });
    }

    template <typename Func>
    void for_each_attribute(const AmbienceComponent& component, Func&& func) const
    {
        func(reflattr<const glm::vec3, true, true>{.name="colour", .display_name="Colour", .value=&component.colour, .metadata={{ "is_colour", "" }} });
        func(reflattr<const float, true, true>{.name="brightness", .display_name="Brightness", .value=&component.brightness, .metadata={} });
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
        func(reflattr<game::entity, true, true>{.name="hovered_square_entity", .display_name="Hovered Square Entity", .value=&component.hovered_square_entity, .metadata={} });
        func(reflattr<game::entity, true, true>{.name="clicked_square_entity", .display_name="Clicked Square Entity", .value=&component.clicked_square_entity, .metadata={} });
        func(reflattr<glm::ivec2, true, true>{.name="hovered_square", .display_name="Hovered Square", .value=&component.hovered_square, .metadata={} });
        func(reflattr<std::optional<glm::ivec2>, true, true>{.name="clicked_square", .display_name="Clicked Square", .value=&component.clicked_square, .metadata={} });
    }

    template <typename Func>
    void for_each_attribute(const GameGridSingleton& component, Func&& func) const
    {
        func(reflattr<const game::entity, true, true>{.name="hovered_square_entity", .display_name="Hovered Square Entity", .value=&component.hovered_square_entity, .metadata={} });
        func(reflattr<const game::entity, true, true>{.name="clicked_square_entity", .display_name="Clicked Square Entity", .value=&component.clicked_square_entity, .metadata={} });
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
        func(reflattr<std::unordered_map<glm::ivec2, game::entity>, true, true>{.name="tiles", .display_name="Tiles", .value=&component.tiles, .metadata={} });
    }

    template <typename Func>
    void for_each_attribute(const TileMapSingleton& component, Func&& func) const
    {
        func(reflattr<const std::unordered_map<glm::ivec2, game::entity>, true, true>{.name="tiles", .display_name="Tiles", .value=&component.tiles, .metadata={} });
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
        func(reflattr<game::entity, true, true>{.name="camera_entity", .display_name="Camera Entity", .value=&component.camera_entity, .metadata={} });
    }

    template <typename Func>
    void for_each_attribute(const CameraSingleton& component, Func&& func) const
    {
        func(reflattr<const game::entity, true, true>{.name="camera_entity", .display_name="Camera Entity", .value=&component.camera_entity, .metadata={} });
    }
};


template <typename Func>
void for_each_component(Func&& func)
{
    func(reflcomp<Runtime>{});
    func(reflcomp<Event>{});
    func(reflcomp<NameComponent>{});
    func(reflcomp<Transform3DComponent>{});
    func(reflcomp<StaticModelComponent>{});
    func(reflcomp<AnimatedModelComponent>{});
    func(reflcomp<ScriptComponent>{});
    func(reflcomp<Camera3DComponent>{});
    func(reflcomp<PathComponent>{});
    func(reflcomp<LightComponent>{});
    func(reflcomp<SunComponent>{});
    func(reflcomp<AmbienceComponent>{});
    func(reflcomp<InputSingleton>{});
    func(reflcomp<GameGridSingleton>{});
    func(reflcomp<TileMapSingleton>{});
    func(reflcomp<CameraSingleton>{});
}

}
