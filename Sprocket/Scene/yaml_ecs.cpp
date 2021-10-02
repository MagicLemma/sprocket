#include "yaml_ecs.h"

#include <type_traits>

namespace YAML {

Node convert<spkt::entity>::encode(const spkt::entity& rhs)
{
    Node n;
    n = static_cast<std::underlying_type_t<spkt::entity>>(rhs);
    return n;
}

bool convert<spkt::entity>::decode(const Node& node, spkt::entity& rhs)
{
    if (!node.IsScalar())
        return false;

    rhs = static_cast<spkt::entity>(node.as<std::underlying_type_t<spkt::entity>>());
    return true;
}

}

namespace spkt {

YAML::Emitter& operator<<(YAML::Emitter& out, const spkt::entity& i)
{
    out << static_cast<std::underlying_type_t<spkt::entity>>(i);
    return out;
}

}