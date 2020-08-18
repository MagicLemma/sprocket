#pragma once
#include "Maths.h"

#include <yaml-cpp/yaml.h>

namespace YAML {

using namespace Sprocket;

template<>
struct convert<Maths::vec2>
{
    static Node encode(const Maths::vec2& rhs);
    static bool decode(const Node& node, Maths::vec2& rhs);
};

template<>
struct convert<Sprocket::Maths::vec3>
{
    static Node encode(const Sprocket::Maths::vec3& rhs);
    static bool decode(const Node& node, Sprocket::Maths::vec3& rhs);
};

template<>
struct convert<Sprocket::Maths::quat>
{
    static Node encode(const Sprocket::Maths::quat& rhs);
    static bool decode(const Node& node, Sprocket::Maths::quat& rhs);
};

template<>
struct convert<Sprocket::Maths::mat4>
{
    static Node encode(const Sprocket::Maths::mat4& rhs);
    static bool decode(const Node& node, Sprocket::Maths::mat4& rhs);
};

}

namespace Sprocket {
 
YAML::Emitter& operator<<(YAML::Emitter& out, const Maths::vec3& v);
YAML::Emitter& operator<<(YAML::Emitter& out, const Maths::quat& q);
YAML::Emitter& operator<<(YAML::Emitter& out, const Maths::mat4& m);

}