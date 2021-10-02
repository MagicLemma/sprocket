#include "yaml_ecs.h"

#include <type_traits>

namespace YAML {

using namespace {{Global::namespace}};

Node convert<entity>::encode(const entity& rhs)
{
    Node n;
    n = static_cast<std::underlying_type_t<entity>>(rhs);
    return n;
}

bool convert<entity>::decode(const Node& node, entity& rhs)
{
    if (!node.IsScalar())
        return false;

    rhs = static_cast<entity>(node.as<std::underlying_type_t<entity>>());
    return true;
}

}

namespace {{Global::namespace}} {

YAML::Emitter& operator<<(YAML::Emitter& out, const entity& i)
{
    out << static_cast<std::underlying_type_t<entity>>(i);
    return out;
}

}