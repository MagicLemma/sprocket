#pragma once
#include "Maths.h"
#include <queue>
#include <string>
#include <utility>
#include <array>
#include <unordered_map>
#include <optional>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

#include "apecs.hpp"

namespace Sprocket {

// Components
DATAMATIC_BEGIN
struct {{Comp::name}}
{
    {{Attr::type}} {{Attr::name}} = {{Attr::default}};
};

DATAMATIC_END

}
