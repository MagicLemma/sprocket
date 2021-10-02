#pragma once
#include <apecs.hpp>
{{Global::include_list}}

{{Global::forward_decl_list}}

namespace spkt {

using entity = apx::entity;
const auto null = apx::null;

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
using registry = apx::registry<
DATAMATIC_BEGIN
    {{Comp::name}}{{Comp::if_not_last(",")}}
DATAMATIC_END
>;

}
