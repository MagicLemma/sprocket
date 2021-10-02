#pragma once
#include "ecs.h"

#include <yaml-cpp/yaml.h>

namespace YAML {

template<>
struct convert<spkt::entity>
{
    static Node encode(const spkt::entity& rhs);
    static bool decode(const Node& node, spkt::entity& rhs);
};

}

namespace spkt {

YAML::Emitter& operator<<(YAML::Emitter& out, const entity& i);

}
