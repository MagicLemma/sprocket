#pragma once
#include "Maths.h"
#include <queue>
#include <string>
#include <utility>
#include <array>
#include <unordered_map>
#include <map>
#include <optional>
#include <memory>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

#include "apecs.hpp"

namespace spkt {
namespace lua { class Script; }
struct physics_runtime;
struct rigid_body_runtime;
struct collider_runtime;

// Components
DATAMATIC_BEGIN
struct {{Comp::name}}
{
    {{Attr::type}} {{Attr::name}} = {{Attr::default}};
};

DATAMATIC_END

}
