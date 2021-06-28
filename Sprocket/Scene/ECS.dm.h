#pragma once
#include "Components.h"

#include "apecs.hpp"

namespace spkt {

using registry = apx::registry<
DATAMATIC_BEGIN
    Sprocket::{{Comp::name}}{{Comp::if_not_last(",")}}
DATAMATIC_END
>;

using entity = typename registry::handle_type;
using identifier = apx::entity;

inline constexpr entity null;

}