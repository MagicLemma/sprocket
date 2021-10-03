#include "Yaml.h"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace YAML {

Node convert<glm::vec2>::encode(const glm::vec2& rhs)
{
    Node n;
    n.push_back(rhs.x);
    n.push_back(rhs.y);
    return n;
}

bool convert<glm::vec2>::decode(const Node& node, glm::vec2& rhs)
{
    if (!node.IsSequence() || node.size() != 2)
        return false;

    rhs.x = node[0].as<float>();
    rhs.y = node[1].as<float>();
    return true;
}

Node convert<glm::ivec2>::encode(const glm::ivec2& rhs)
{
    Node n;
    n.push_back(rhs.x);
    n.push_back(rhs.y);
    return n;
}

bool convert<glm::ivec2>::decode(const Node& node, glm::ivec2& rhs)
{
    if (!node.IsSequence() || node.size() != 2)
        return false;

    rhs.x = node[0].as<int>();
    rhs.y = node[1].as<int>();
    return true;
}

Node convert<glm::vec3>::encode(const glm::vec3& rhs)
{
    Node n;
    n.push_back(rhs.x);
    n.push_back(rhs.y);
    n.push_back(rhs.z);
    return n;
}

bool convert<glm::vec3>::decode(const Node& node, glm::vec3& rhs)
{
    if (!node.IsSequence() || node.size() != 3)
        return false;

    rhs.x = node[0].as<float>();
    rhs.y = node[1].as<float>();
    rhs.z = node[2].as<float>();
    return true;
}

Node convert<glm::quat>::encode(const glm::quat& rhs)
{
    Node n;
    n.push_back(rhs.w);
    n.push_back(rhs.x);
    n.push_back(rhs.y);
    n.push_back(rhs.z);
    return n;
}

bool convert<glm::quat>::decode(const Node& node, glm::quat& rhs)
{
    if (!node.IsSequence() || node.size() != 4)
        return false;

    rhs.w = node[0].as<float>();
    rhs.x = node[1].as<float>();
    rhs.y = node[2].as<float>();
    rhs.z = node[3].as<float>();
    return true;
}

Node convert<glm::mat4>::encode(const glm::mat4& rhs)
{
    Node n;
    n.push_back(rhs[0][0]);
    n.push_back(rhs[0][1]);
    n.push_back(rhs[0][2]);
    n.push_back(rhs[0][3]);

    n.push_back(rhs[1][0]);
    n.push_back(rhs[1][1]);
    n.push_back(rhs[1][2]);
    n.push_back(rhs[1][3]);

    n.push_back(rhs[2][0]);
    n.push_back(rhs[2][1]);
    n.push_back(rhs[2][2]);
    n.push_back(rhs[2][3]);

    n.push_back(rhs[3][0]);
    n.push_back(rhs[3][1]);
    n.push_back(rhs[3][2]);
    n.push_back(rhs[3][3]);
    return n;
}

bool convert<glm::mat4>::decode(const Node& node, glm::mat4& rhs)
{
    if (!node.IsSequence() || node.size() != 16)
        return false;

    rhs[0][0] = node[0].as<float>();
    rhs[0][1] = node[1].as<float>();
    rhs[0][2] = node[2].as<float>();
    rhs[0][3] = node[3].as<float>();
    rhs[1][0] = node[4].as<float>();
    rhs[1][1] = node[5].as<float>();
    rhs[1][2] = node[6].as<float>();
    rhs[1][3] = node[7].as<float>();
    rhs[2][0] = node[8].as<float>();
    rhs[2][1] = node[9].as<float>();
    rhs[2][2] = node[10].as<float>();
    rhs[2][3] = node[11].as<float>();
    rhs[3][0] = node[12].as<float>();
    rhs[3][1] = node[13].as<float>();
    rhs[3][2] = node[14].as<float>();
    rhs[3][3] = node[15].as<float>();
    return true;
}

Node convert<apx::entity>::encode(const apx::entity& rhs)
{
    Node n;
    n = static_cast<std::underlying_type_t<apx::entity>>(rhs);
    return n;
}

bool convert<apx::entity>::decode(const Node& node, apx::entity& rhs)
{
    if (!node.IsScalar())
        return false;

    rhs = static_cast<apx::entity>(node.as<std::underlying_type_t<apx::entity>>());
    return true;
}

YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec2& v)
{
    out << YAML::Flow << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
    return out;
}

YAML::Emitter& operator<<(YAML::Emitter& out, const glm::ivec2& v)
{
    out << YAML::Flow << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
    return out;
}

YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v)
{
    out << YAML::Flow << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
    return out;
}

YAML::Emitter& operator<<(YAML::Emitter& out, const glm::quat& q)
{
    out << YAML::Flow << YAML::BeginSeq << q.w << q.x << q.y << q.z << YAML::EndSeq;
    return out;
}

YAML::Emitter& operator<<(YAML::Emitter& out, const glm::mat4& m)
{
    out << YAML::Flow << YAML::BeginSeq
        << m[0][0] << m[0][1] << m[0][2] << m[0][3]
        << m[1][0] << m[1][1] << m[1][2] << m[1][3]
        << m[2][0] << m[2][1] << m[2][2] << m[2][3]
        << m[3][0] << m[3][1] << m[3][2] << m[3][3]
        << YAML::EndSeq;
    return out;
}

YAML::Emitter& operator<<(YAML::Emitter& out, const apx::entity& e)
{
    out << std::underlying_type_t<apx::entity>(e);
    return out;
}

}