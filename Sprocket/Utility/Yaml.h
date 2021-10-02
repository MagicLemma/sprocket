#pragma once
#include <Sprocket/Scene/ecs.h>

#include <glm/glm.hpp>
#include <yaml-cpp/yaml.h>

namespace YAML {

using namespace spkt;

template<>
struct convert<glm::vec2>
{
    static Node encode(const glm::vec2& rhs);
    static bool decode(const Node& node, glm::vec2& rhs);
};

template<>
struct convert<glm::ivec2>
{
    static Node encode(const glm::ivec2& rhs);
    static bool decode(const Node& node, glm::ivec2& rhs);
};

template<>
struct convert<glm::vec3>
{
    static Node encode(const glm::vec3& rhs);
    static bool decode(const Node& node, glm::vec3& rhs);
};

template<>
struct convert<glm::quat>
{
    static Node encode(const glm::quat& rhs);
    static bool decode(const Node& node, glm::quat& rhs);
};

template<>
struct convert<glm::mat4>
{
    static Node encode(const glm::mat4& rhs);
    static bool decode(const Node& node, glm::mat4& rhs);
};

template<>
struct convert<spkt::entity>
{
    static Node encode(const spkt::entity& rhs);
    static bool decode(const Node& node, spkt::entity& rhs);
};

template <typename K, typename V, typename... Rest>
struct convert<std::unordered_map<K, V, Rest...>>
{
    using map_t = std::unordered_map<K, V, Rest...>;
    static Node encode(const map_t& rhs)
    {
        Node node(NodeType::Map);
        for (const auto& [k, v] : rhs) {
            node.force_insert(k, v);
        }
        return node;
    }

    static bool decode(const Node& node, map_t& rhs)
    {
        if (!node.IsMap()) {
            return false;
        }

        rhs.clear();
        for (const auto& elem : node) {
            rhs[elem.first.as<K>()] = elem.second.as<V>();
        }
        return true;
    }
};

}

namespace spkt {

YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec2& v);
YAML::Emitter& operator<<(YAML::Emitter& out, const glm::ivec2& v);
YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v);
YAML::Emitter& operator<<(YAML::Emitter& out, const glm::quat& q);
YAML::Emitter& operator<<(YAML::Emitter& out, const glm::mat4& m);

template <typename K, typename V, typename... Rest>
YAML::Emitter& operator<<(YAML::Emitter& out, const std::unordered_map<K, V, Rest...>& m)
{
    out << YAML::BeginMap;
    for (const auto& [k, v] : m) {
        out << YAML::Key << k << YAML::Value << v;
    }
    out << YAML::EndMap;
    return out;
}

}