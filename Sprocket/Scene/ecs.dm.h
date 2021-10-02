#pragma once
#include <apecs.hpp>
{{Global::include_list}}

{{Global::forward_decl_list}}

namespace spkt {

using entity = apx::entity;
const auto null = apx::null;

// Components
DATAMATIC_BEGIN
struct {{Comp::name}}
{
    {{Attr::type}} {{Attr::name}} = {{Attr::default}};
};

DATAMATIC_END
using registry = apx::registry<
DATAMATIC_BEGIN
    {{Comp::name}}{{Comp::if_not_last(",")}}
DATAMATIC_END
>;

}
