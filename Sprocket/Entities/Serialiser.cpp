#include "Serialiser.h"
#include "Log.h"
#include "Components.h"
#include "Maths.h"

#include <yaml-cpp/yaml.h>

#include <fstream>

namespace YAML {

template<>
struct convert<Sprocket::Maths::vec2>
{
    static Node encode(const Sprocket::Maths::vec2& rhs)
    {
        Node n;
        n.push_back(rhs.x);
        n.push_back(rhs.y);
        return n;
    }

    static bool decode(const Node& node, Sprocket::Maths::vec2& rhs)
    {
        if (!node.IsSequence() || node.size() != 2)
            return false;

        rhs.x = node[0].as<float>();
        rhs.y = node[1].as<float>();
        return true;
    }
};

template<>
struct convert<Sprocket::Maths::vec3>
{
    static Node encode(const Sprocket::Maths::vec3& rhs)
    {
        Node n;
        n.push_back(rhs.x);
        n.push_back(rhs.y);
        n.push_back(rhs.z);
        return n;
    }

    static bool decode(const Node& node, Sprocket::Maths::vec3& rhs)
    {
        if (!node.IsSequence() || node.size() != 3)
            return false;

        rhs.x = node[0].as<float>();
        rhs.y = node[1].as<float>();
        rhs.z = node[2].as<float>();
        return true;
    }
};

template<>
struct convert<Sprocket::Maths::quat>
{
    static Node encode(const Sprocket::Maths::quat& rhs)
    {
        Node n;
        n.push_back(rhs.w);
        n.push_back(rhs.x);
        n.push_back(rhs.y);
        n.push_back(rhs.z);
        return n;
    }

    static bool decode(const Node& node, Sprocket::Maths::quat& rhs)
    {
        if (!node.IsSequence() || node.size() != 4)
            return false;

        rhs.w = node[0].as<float>();
        rhs.x = node[1].as<float>();
        rhs.y = node[2].as<float>();
        rhs.z = node[3].as<float>();
        return true;
    }
};

template<>
struct convert<Sprocket::Maths::mat4>
{
    static Node encode(const Sprocket::Maths::mat4& rhs)
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

    static bool decode(const Node& node, Sprocket::Maths::mat4& rhs)
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
};

}

namespace Sprocket {
namespace {

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

template <typename T>
YAML::Emitter& operator<<(YAML::Emitter& out, const std::queue<T> q)
{
    auto copy = q;
    out << YAML::BeginSeq;
    while (!copy.empty()) {
        auto obj = copy.front();
        copy.pop();
        out << obj;
    }
    out << YAML::EndSeq;
    return out;
}

template <typename T>
struct MapEntry
{
    const char* key;
    T           value;

    MapEntry(const char* key_, const T& value_) : key(key_), value(value_) {}
};

template <typename T>
YAML::Emitter& operator<<(YAML::Emitter& out, const MapEntry<T>& entry)
{
    out << YAML::Key << entry.key << YAML::Value << entry.value;
    return out;
}

}

void Serialiser::Serialise(const std::string& file)
{
    YAML::Emitter out;
    out << YAML::BeginMap;
    out << YAML::Key << "Scene";
    out << YAML::Value << "Scene Name";

    out << YAML::Key << "Entities" << YAML::BeginSeq;
    d_scene->All([&](Entity& entity) {
        if (entity.Has<TemporaryComponent>()) { return; }

        out << YAML::BeginMap;
        if (entity.Has<NameComponent>()) {
            const auto& name = entity.Get<NameComponent>();
            out << YAML::Key << "Name" << YAML::BeginMap;
            out << MapEntry("Name", name.name);
            out << YAML::EndMap;
        }
        if (entity.Has<TransformComponent>()) {
            const auto& transform = entity.Get<TransformComponent>();
            out << YAML::Key << "Transform";
            out << YAML::BeginMap;
            out << MapEntry("Position", transform.position);
            out << MapEntry("Orientation", transform.orientation);
            out << YAML::EndMap;
        }
        if (entity.Has<ModelComponent>()) {
            const auto& model = entity.Get<ModelComponent>();
            out << YAML::Key << "Model";
            out << YAML::BeginMap;
            out << MapEntry("Model", model.model);
            out << MapEntry("Scale", model.scale);
            out << MapEntry("Texture", model.texture);
            out << MapEntry("ShineDamper", model.shineDamper);
            out << MapEntry("Reflectivity", model.reflectivity);
            out << YAML::EndMap;
        }
        if (entity.Has<PhysicsComponent>()) {
            const auto& physics = entity.Get<PhysicsComponent>();
            out << YAML::Key << "Physics";
            out << YAML::BeginMap;
            out << MapEntry("Velocity", physics.velocity);
            out << MapEntry("Gravity", physics.gravity);
            out << MapEntry("Frozen", physics.frozen);
            out << MapEntry("Collider", static_cast<int>(physics.collider));
            out << MapEntry("HalfExtents", physics.halfExtents);
            out << MapEntry("Radius", physics.radius);
            out << MapEntry("Height", physics.height);
            out << MapEntry("Mass", physics.mass);
            out << MapEntry("Bounciness", physics.bounciness);
            out << MapEntry("FrictionCoefficient", physics.frictionCoefficient);
            out << MapEntry("RollingResistance", physics.rollingResistance);
            out << YAML::EndMap;
        }
        if (entity.Has<ScriptComponent>()) {
            const auto& script = entity.Get<ScriptComponent>();
            out << YAML::Key << "Script";
            out << YAML::BeginMap;
            out << MapEntry("Script", script.script);
            out << MapEntry("Active", script.active);
            out << YAML::EndMap;
        }
        if (entity.Has<PlayerComponent>()) {
            const auto& player = entity.Get<PlayerComponent>();
            out << YAML::Key << "Player";
            out << YAML::BeginMap;
            out << MapEntry("MovingForwards", player.movingForwards);
            out << MapEntry("MovingBackwards", player.movingBackwards);
            out << MapEntry("MovingLeft", player.movingLeft);
            out << MapEntry("MovingRight", player.movingRight);
            out << MapEntry("Jumping", player.jumping);
            out << MapEntry("Direction", player.direction);
            out << MapEntry("Yaw", player.yaw);
            out << MapEntry("Pitch", player.pitch);
            out << YAML::EndMap;
        }
        if (entity.Has<CameraComponent>()) {
            const auto& camera = entity.Get<CameraComponent>();
            out << YAML::Key << "Camera";
            out << YAML::BeginMap;
            out << MapEntry("Projection", camera.projection);
            out << MapEntry("Pitch", camera.pitch);
            out << YAML::EndMap;
        }
        if (entity.Has<SelectComponent>()) {
            const auto& select = entity.Get<SelectComponent>();
            // No need to store the values.
            out << YAML::Key << "Select" << true;
        }
        if (entity.Has<PathComponent>()) {
            const auto& path = entity.Get<PathComponent>();
            out << YAML::Key << "Path";
            out << YAML::BeginMap;
            out << MapEntry("Markers", path.markers);
            out << MapEntry("Speed", path.speed);
            out << YAML::EndMap;
        }
        if (entity.Has<GridComponent>()) {
            const auto& grid = entity.Get<GridComponent>();
            out << YAML::Key << "Grid";
            out << YAML::BeginMap;
            out << MapEntry("X", grid.x);
            out << MapEntry("Z", grid.z);
            out << YAML::EndMap;
        }
        out << YAML::EndMap;
    });
    out << YAML::EndSeq;
    out << YAML::EndMap;

    std::ofstream fout(file);
    fout << out.c_str();
}

void Serialiser::Deserialise(const std::string& file)
{
    d_scene->Clear();

    std::ifstream stream(file);
    std::stringstream sstream;
    sstream << stream.rdbuf();

    YAML::Node data = YAML::Load(sstream.str());
    if (!data["Entities"]) {
        return; // TODO: Error checking
    }

    auto entities = data["Entities"];
    for (auto entity : entities) {
        Entity e = d_scene->NewEntity();

        auto name = entity["Name"];
        if (name) {
            NameComponent nc;
            nc.name = name["Name"].as<std::string>();
            e.Add(nc);
        }

        auto transform = entity["Transform"];
        if (transform) {
            TransformComponent tc;
            tc.position = transform["Position"].as<Maths::vec3>();
            tc.orientation = transform["Orientation"].as<Maths::quat>();
            e.Add(tc);
        }

        auto model = entity["Model"];
        if (model) {
            ModelComponent mc;
            mc.model = model["Model"].as<std::string>();
            mc.scale = model["Scale"].as<float>();
            mc.texture = model["Texture"].as<std::string>();
            mc.shineDamper = model["ShineDamper"].as<float>();
            mc.reflectivity = model["Reflectivity"].as<float>();
            e.Add(mc);
        }

        auto physics = entity["Physics"];
        if (physics) {
            PhysicsComponent pc;
            pc.velocity = physics["Velocity"].as<Maths::vec3>();
            pc.gravity = physics["Gravity"].as<bool>();
            pc.frozen = physics["Frozen"].as<bool>();
            pc.collider = static_cast<Collider>(physics["Collider"].as<int>());
            pc.halfExtents = physics["HalfExtents"].as<Maths::vec3>();
            pc.radius = physics["Radius"].as<float>();
            pc.height = physics["Height"].as<float>();
            pc.mass = physics["Mass"].as<float>();
            pc.bounciness = physics["Bounciness"].as<float>();
            pc.frictionCoefficient = physics["FrictionCoefficient"].as<float>();
            pc.rollingResistance = physics["RollingResistance"].as<float>();
            e.Add(pc);
        }

        auto player = entity["Player"];
        if (player) {
            PlayerComponent pc;
            pc.movingForwards = player["MovingForwards"].as<bool>();
            pc.movingBackwards = player["MovingBackwards"].as<bool>();
            pc.movingLeft = player["MovingLeft"].as<bool>();
            pc.movingRight = player["MovingRight"].as<bool>();
            pc.jumping = player["Jumping"].as<bool>();
            pc.direction = player["Direction"].as<Maths::vec3>();
            pc.yaw = player["Yaw"].as<float>();
            pc.pitch = player["Pitch"].as<float>();
            e.Add(pc);
        }

        auto camera = entity["Camera"];
        if (camera) {
            CameraComponent cc;
            cc.projection = camera["Projection"].as<Maths::mat4>();
            cc.pitch = camera["Pitch"].as<float>();
            e.Add(cc);
        }

        auto select = entity["Select"];
        if (select) {
            SelectComponent sc;
            e.Add(sc);
        }

        auto path = entity["Path"];
        if (path) {
            PathComponent pc;
            // TODO Sort this
            //pc.markers = path["Markers"].as<std::queue<Maths::vec3>>();
            pc.speed = path["Speed"].as<float>();
            e.Add(pc);
        }

        auto grid = entity["Grid"];
        if (grid) {
            GridComponent gc;
            gc.x = grid["X"].as<int>();
            gc.z = grid["Z"].as<int>();
            e.Add(gc);
        }

        // This needs to get added last as scripts may access other components.
        auto script = entity["Script"];
        if (script) {
            ScriptComponent sc;
            sc.script = script["Script"].as<std::string>();
            sc.active = script["Active"].as<bool>();
            e.Add(sc);
        }
    }
}

}