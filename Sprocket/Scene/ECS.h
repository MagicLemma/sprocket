#pragma once
#include "Components.h"

#include "apecs.hpp"

namespace spkt {

using registry = apx::registry;
using entity = apx::handle;

inline constexpr entity null = apx::null_handle;

}
