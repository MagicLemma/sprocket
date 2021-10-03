#pragma once
#include "ecs.h"

#include <yaml-cpp/yaml.h>

namespace YAML {

template<>
struct convert<{{Global::namespace}}::entity>
{
    static Node encode(const {{Global::namespace}}::entity& rhs);
    static bool decode(const Node& node, {{Global::namespace}}::entity& rhs);
};

}

namespace {{Global::namespace}} {

YAML::Emitter& operator<<(YAML::Emitter& out, const entity& i);

}