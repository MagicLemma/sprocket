#include "Serialiser.h"
#include "Log.h"
#include "Components.h"

#include <yaml-cpp/yaml.h>

#include <fstream>

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
        out << YAML::Key << "ID" << YAML::Value << entity.Id();
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
            out << YAML::Key << "Select";
            out << YAML::BeginMap;
            out << MapEntry("Selected", select.selected);
            out << MapEntry("Hovered", select.hovered);
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
    SPKT_LOG_ERROR("Deserialising not yet implemented");
    // TODO
}

}