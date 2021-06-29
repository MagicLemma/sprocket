#pragma once
#include "Components.h"

#include "apecs.hpp"

namespace spkt {

using registry = apx::registry<
DATAMATIC_BEGIN
    spkt::{{Comp::name}}{{Comp::if_not_last(",")}}
DATAMATIC_END
>;

using entity = typename registry::handle_type;

inline constexpr entity null;

}