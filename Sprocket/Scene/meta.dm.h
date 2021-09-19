#pragma once
#include <Sprocket/Scene/ecs.h>

#include <string_view>

namespace spkt {

template <typename T>
struct reflection;

DATAMATIC_BEGIN
template <>
struct reflection<{{Comp::name}}>
{
    static constexpr std::string_view component_name = "{{Comp::name}}";
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