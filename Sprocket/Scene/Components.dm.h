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

using entity = apx::entity;
const auto null = apx::null;

struct physics_runtime;
struct rigid_body_runtime;
struct collider_runtime;

struct particle
{
    glm::vec3 position      = {0.0, 0.0, 0.0};
    glm::vec3 velocity      = {0.0, 0.0, 0.0};
    glm::vec3 acceleration  = {0.0, 0.0, 0.0};
    glm::vec3 scale         = {1.0, 1.0, 1.0};
    double life = -1.0;
};

static constexpr std::size_t NUM_PARTICLES = 10000;

// Components
DATAMATIC_BEGIN
struct {{Comp::name}}
{
    {{Attr::type}} {{Attr::name}} = {{Attr::default}};
};

DATAMATIC_END
using registry = apx::fixed_registry<
DATAMATIC_BEGIN
    {{Comp::name}}{{Comp::if_not_last(",")}}
DATAMATIC_END
>;

using handle = typename registry::handle_type;

}
