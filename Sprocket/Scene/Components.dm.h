#pragma once
#include "Maths.h"
#include <queue>
#include <string>
#include <utility>
#include <array>
#include <unordered_map>
#include <optional>
#include <memory>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

#include "apecs.hpp"

namespace Sprocket {
namespace lua { class Script; }
class collider_runtime;

// Components
DATAMATIC_BEGIN
struct {{Comp::name}}
{
    {{Attr::type}} {{Attr::name}} = {{Attr::default}};
};

DATAMATIC_END

}
