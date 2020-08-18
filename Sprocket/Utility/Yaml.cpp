#include "Yaml.h"

namespace YAML {

using namespace Sprocket;

Node convert<Maths::vec2>::encode(const Maths::vec2& rhs)
{
    Node n;
    n.push_back(rhs.x);
    n.push_back(rhs.y);
    return n;
}

bool convert<Maths::vec2>::decode(const Node& node, Maths::vec2& rhs)
{
    if (!node.IsSequence() || node.size() != 2)
        return false;

    rhs.x = node[0].as<float>();
    rhs.y = node[1].as<float>();
    return true;
}

Node convert<Maths::vec3>::encode(const Maths::vec3& rhs)
{
    Node n;
    n.push_back(rhs.x);
    n.push_back(rhs.y);
    n.push_back(rhs.z);
    return n;
}

bool convert<Maths::vec3>::decode(const Node& node, Maths::vec3& rhs)
{
    if (!node.IsSequence() || node.size() != 3)
        return false;

    rhs.x = node[0].as<float>();
    rhs.y = node[1].as<float>();
    rhs.z = node[2].as<float>();
    return true;
}

Node convert<Maths::quat>::encode(const Maths::quat& rhs)
{
    Node n;
    n.push_back(rhs.w);
    n.push_back(rhs.x);
    n.push_back(rhs.y);
    n.push_back(rhs.z);
    return n;
}

bool convert<Maths::quat>::decode(const Node& node, Maths::quat& rhs)
{
    if (!node.IsSequence() || node.size() != 4)
        return false;

    rhs.w = node[0].as<float>();
    rhs.x = node[1].as<float>();
    rhs.y = node[2].as<float>();
    rhs.z = node[3].as<float>();
    return true;
}

Node convert<Maths::mat4>::encode(const Maths::mat4& rhs)
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

bool convert<Maths::mat4>::decode(const Node& node, Maths::mat4& rhs)
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

}

namespace Sprocket {

YAML::Emitter& operator<<(YAML::Emitter& out, const Maths::vec3& v)
{
    out << YAML::Flow;
    out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
    return out;
}

YAML::Emitter& operator<<(YAML::Emitter& out, const Maths::quat& q)
{
    out << YAML::Flow;
    out << YAML::BeginSeq << q.w << q.x << q.y << q.z << YAML::EndSeq;
    return out;
}

YAML::Emitter& operator<<(YAML::Emitter& out, const Maths::mat4& m)
{
    out << YAML::Flow;
    out << YAML::BeginSeq << m[0][0] << m[0][1] << m[0][2] << m[0][3]
                          << m[1][0] << m[1][1] << m[1][2] << m[1][3]
                          << m[2][0] << m[2][1] << m[2][2] << m[2][3]
                          << m[3][0] << m[3][1] << m[3][2] << m[3][3]
                          << YAML::EndSeq;
    return out;
}

}