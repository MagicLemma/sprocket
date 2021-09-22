#pragma once
#include <Sprocket/Scene/ecs.h>

#include <string_view>
#include <string>

namespace spkt {

template <typename T, bool Savable, bool Scriptable>
struct attribute_reflection
{
    const std::string_view                             name;
    const std::string_view                             display_name;
    T*                                                 value;
    const std::unordered_map<std::string, std::string> metadata;

    static constexpr bool  is_savable()    { return Savable; }
    static constexpr bool  is_scriptable() { return Scriptable; }
};

template <typename T, bool Savable, bool Scriptable>
struct const_attribute_reflection
{
    const std::string_view                             name;
    const std::string_view                             display_name;
    const T* const                                     value;
    const std::unordered_map<std::string, std::string> metadata;

    static constexpr bool  is_savable()    { return Savable; }
    static constexpr bool  is_scriptable() { return Scriptable; }
};

template <typename T>
struct reflection;

DATAMATIC_BEGIN
template <>
struct reflection<{{Comp::name}}>
{
    static constexpr const char* name          = "{{Comp::name}}";
    static constexpr const char* display_name  = "{{Comp::display_name}}";

    static constexpr bool        is_savable()    { return {{Comp::is_savable}}; }
    static constexpr bool        is_scriptable() { return {{Comp::is_scriptable}}; }

    template <typename Func>
    void attributes({{Comp::name}}& component, Func&& func)
    {
        func(attribute_reflection<{{Attr::type}}, {{Attr::is_savable}}, {{Attr::is_scriptable}}>{.name="{{Attr::name}}", .display_name="{{Attr::display_name}}", .value=&component.{{Attr::name}}, .metadata={{Attr::get_metadata}} });
    }

    template <typename Func>
    void attributes(const {{Comp::name}}& component, Func&& func)
    {
        func(const_attribute_reflection<{{Attr::type}}, {{Attr::is_savable}}, {{Attr::is_scriptable}}>{.name="{{Attr::name}}", .display_name="{{Attr::display_name}}", .value=&component.{{Attr::name}}, .metadata={{Attr::get_metadata}} });
    }
};

DATAMATIC_END

template <typename Func>
void for_each_reflect(Func&& func)
{
DATAMATIC_BEGIN
    func(reflection<{{Comp::name}}>{});
DATAMATIC_END
}

}