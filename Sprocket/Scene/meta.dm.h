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

DATAMATIC_BEGIN
template <>
struct reflection<{{Comp::name}}>
{
    static constexpr const char* name          = "{{Comp::name}}";
    static constexpr const char* display_name  = "{{Comp::display_name}}";
    static constexpr bool        is_savable    = {{Comp::is_savable}};
    static constexpr bool        is_scriptable = {{Comp::is_scriptable}};

    template <typename Func>
    void attributes(Func&& func, const {{Comp::name}}& component)
    {
        func(attribute_reflection<{{Attr::type}}>{.name="{{Attr::name}}", .is_savable={{Attr::is_savable}}, .is_scriptable={{Attr::is_scriptable}}, .value=&component.{{Attr::name}}});
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