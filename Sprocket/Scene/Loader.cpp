// GENERATED FILE
#include "Loader.h"
#include "Log.h"
#include "Components.h"
#include "Maths.h"
#include "Yaml.h"
#include "Scene.h"

#include <yaml-cpp/yaml.h>
#include <fstream>
#include <memory>

namespace Sprocket {
namespace Loader {

void Save(const std::string& file, ecs::Registry* reg)
{
    YAML::Emitter out;
    out << YAML::BeginMap;
    out << YAML::Key << "Entities" << YAML::BeginSeq;
    for (auto entity : reg->Each()) {
        if (entity.has<TemporaryComponent>()) { return; }
        out << YAML::BeginMap;
        out << YAML::Key << "@GUID" << YAML::Value << entity.id();
        if (entity.has<TemporaryComponent>()) {
            const auto& c = entity.get<TemporaryComponent>();
            out << YAML::Key << "TemporaryComponent" << YAML::BeginMap;
            out << YAML::EndMap;
        }
        if (entity.has<NameComponent>()) {
            const auto& c = entity.get<NameComponent>();
            out << YAML::Key << "NameComponent" << YAML::BeginMap;
            out << YAML::Key << "name" << YAML::Value << c.name;
            out << YAML::EndMap;
        }
        if (entity.has<Transform2DComponent>()) {
            const auto& c = entity.get<Transform2DComponent>();
            out << YAML::Key << "Transform2DComponent" << YAML::BeginMap;
            out << YAML::Key << "position" << YAML::Value << c.position;
            out << YAML::Key << "rotation" << YAML::Value << c.rotation;
            out << YAML::Key << "scale" << YAML::Value << c.scale;
            out << YAML::EndMap;
        }
        if (entity.has<Transform3DComponent>()) {
            const auto& c = entity.get<Transform3DComponent>();
            out << YAML::Key << "Transform3DComponent" << YAML::BeginMap;
            out << YAML::Key << "position" << YAML::Value << c.position;
            out << YAML::Key << "orientation" << YAML::Value << c.orientation;
            out << YAML::Key << "scale" << YAML::Value << c.scale;
            out << YAML::EndMap;
        }
        if (entity.has<ModelComponent>()) {
            const auto& c = entity.get<ModelComponent>();
            out << YAML::Key << "ModelComponent" << YAML::BeginMap;
            out << YAML::Key << "mesh" << YAML::Value << c.mesh;
            out << YAML::Key << "material" << YAML::Value << c.material;
            out << YAML::EndMap;
        }
        if (entity.has<RigidBody3DComponent>()) {
            const auto& c = entity.get<RigidBody3DComponent>();
            out << YAML::Key << "RigidBody3DComponent" << YAML::BeginMap;
            out << YAML::Key << "velocity" << YAML::Value << c.velocity;
            out << YAML::Key << "gravity" << YAML::Value << c.gravity;
            out << YAML::Key << "frozen" << YAML::Value << c.frozen;
            out << YAML::Key << "bounciness" << YAML::Value << c.bounciness;
            out << YAML::Key << "frictionCoefficient" << YAML::Value << c.frictionCoefficient;
            out << YAML::Key << "rollingResistance" << YAML::Value << c.rollingResistance;
            out << YAML::EndMap;
        }
        if (entity.has<BoxCollider3DComponent>()) {
            const auto& c = entity.get<BoxCollider3DComponent>();
            out << YAML::Key << "BoxCollider3DComponent" << YAML::BeginMap;
            out << YAML::Key << "position" << YAML::Value << c.position;
            out << YAML::Key << "orientation" << YAML::Value << c.orientation;
            out << YAML::Key << "mass" << YAML::Value << c.mass;
            out << YAML::Key << "halfExtents" << YAML::Value << c.halfExtents;
            out << YAML::Key << "applyScale" << YAML::Value << c.applyScale;
            out << YAML::EndMap;
        }
        if (entity.has<SphereCollider3DComponent>()) {
            const auto& c = entity.get<SphereCollider3DComponent>();
            out << YAML::Key << "SphereCollider3DComponent" << YAML::BeginMap;
            out << YAML::Key << "position" << YAML::Value << c.position;
            out << YAML::Key << "orientation" << YAML::Value << c.orientation;
            out << YAML::Key << "mass" << YAML::Value << c.mass;
            out << YAML::Key << "radius" << YAML::Value << c.radius;
            out << YAML::EndMap;
        }
        if (entity.has<CapsuleCollider3DComponent>()) {
            const auto& c = entity.get<CapsuleCollider3DComponent>();
            out << YAML::Key << "CapsuleCollider3DComponent" << YAML::BeginMap;
            out << YAML::Key << "position" << YAML::Value << c.position;
            out << YAML::Key << "orientation" << YAML::Value << c.orientation;
            out << YAML::Key << "mass" << YAML::Value << c.mass;
            out << YAML::Key << "radius" << YAML::Value << c.radius;
            out << YAML::Key << "height" << YAML::Value << c.height;
            out << YAML::EndMap;
        }
        if (entity.has<ScriptComponent>()) {
            const auto& c = entity.get<ScriptComponent>();
            out << YAML::Key << "ScriptComponent" << YAML::BeginMap;
            out << YAML::Key << "script" << YAML::Value << c.script;
            out << YAML::Key << "active" << YAML::Value << c.active;
            out << YAML::EndMap;
        }
        if (entity.has<Camera3DComponent>()) {
            const auto& c = entity.get<Camera3DComponent>();
            out << YAML::Key << "Camera3DComponent" << YAML::BeginMap;
            out << YAML::Key << "fov" << YAML::Value << c.fov;
            out << YAML::Key << "pitch" << YAML::Value << c.pitch;
            out << YAML::EndMap;
        }
        if (entity.has<SelectComponent>()) {
            const auto& c = entity.get<SelectComponent>();
            out << YAML::Key << "SelectComponent" << YAML::BeginMap;
            out << YAML::EndMap;
        }
        if (entity.has<PathComponent>()) {
            const auto& c = entity.get<PathComponent>();
            out << YAML::Key << "PathComponent" << YAML::BeginMap;
            out << YAML::Key << "speed" << YAML::Value << c.speed;
            out << YAML::EndMap;
        }
        if (entity.has<GridComponent>()) {
            const auto& c = entity.get<GridComponent>();
            out << YAML::Key << "GridComponent" << YAML::BeginMap;
            out << YAML::Key << "x" << YAML::Value << c.x;
            out << YAML::Key << "z" << YAML::Value << c.z;
            out << YAML::EndMap;
        }
        if (entity.has<LightComponent>()) {
            const auto& c = entity.get<LightComponent>();
            out << YAML::Key << "LightComponent" << YAML::BeginMap;
            out << YAML::Key << "colour" << YAML::Value << c.colour;
            out << YAML::Key << "brightness" << YAML::Value << c.brightness;
            out << YAML::EndMap;
        }
        if (entity.has<SunComponent>()) {
            const auto& c = entity.get<SunComponent>();
            out << YAML::Key << "SunComponent" << YAML::BeginMap;
            out << YAML::Key << "colour" << YAML::Value << c.colour;
            out << YAML::Key << "brightness" << YAML::Value << c.brightness;
            out << YAML::Key << "direction" << YAML::Value << c.direction;
            out << YAML::Key << "shadows" << YAML::Value << c.shadows;
            out << YAML::EndMap;
        }
        if (entity.has<AmbienceComponent>()) {
            const auto& c = entity.get<AmbienceComponent>();
            out << YAML::Key << "AmbienceComponent" << YAML::BeginMap;
            out << YAML::Key << "colour" << YAML::Value << c.colour;
            out << YAML::Key << "brightness" << YAML::Value << c.brightness;
            out << YAML::EndMap;
        }
        if (entity.has<ParticleComponent>()) {
            const auto& c = entity.get<ParticleComponent>();
            out << YAML::Key << "ParticleComponent" << YAML::BeginMap;
            out << YAML::Key << "interval" << YAML::Value << c.interval;
            out << YAML::Key << "velocity" << YAML::Value << c.velocity;
            out << YAML::Key << "velocityNoise" << YAML::Value << c.velocityNoise;
            out << YAML::Key << "acceleration" << YAML::Value << c.acceleration;
            out << YAML::Key << "scale" << YAML::Value << c.scale;
            out << YAML::Key << "life" << YAML::Value << c.life;
            out << YAML::EndMap;
        }
        if (entity.has<MeshAnimationComponent>()) {
            const auto& c = entity.get<MeshAnimationComponent>();
            out << YAML::Key << "MeshAnimationComponent" << YAML::BeginMap;
            out << YAML::Key << "name" << YAML::Value << c.name;
            out << YAML::Key << "time" << YAML::Value << c.time;
            out << YAML::Key << "speed" << YAML::Value << c.speed;
            out << YAML::EndMap;
        }
        out << YAML::EndMap;
    }
    out << YAML::EndSeq;
    out << YAML::EndMap;

    std::ofstream fout(file);
    fout << out.c_str();
}

void Load(const std::string& file, ecs::Registry* reg)
{
    // Must be a clean scene
    u32 count = 0;
    for (ecs::Entity e : reg->Each()) {
        if (!e.has<TemporaryComponent>()) ++count;
    }
    assert(count == 0);

    std::ifstream stream(file);
    std::stringstream sstream;
    sstream << stream.rdbuf();

    YAML::Node data = YAML::Load(sstream.str());

    if (!data["Entities"]) {
        return; // TODO: Error checking
    }

    auto entities = data["Entities"];
    for (auto entity : entities) {
        ecs::Entity e = reg->New(entity["@GUID"].as<guid::GUID>());
        if (auto spec = entity["TemporaryComponent"]) {
            TemporaryComponent c;
            e.add<TemporaryComponent>(c);
        }
        if (auto spec = entity["NameComponent"]) {
            NameComponent c;
            c.name = spec["name"] ? spec["name"].as<std::string>() : "Entity";
            e.add<NameComponent>(c);
        }
        if (auto spec = entity["Transform2DComponent"]) {
            Transform2DComponent c;
            c.position = spec["position"] ? spec["position"].as<glm::vec2>() : glm::vec2{0.0f, 0.0f};
            c.rotation = spec["rotation"] ? spec["rotation"].as<float>() : 0.0f;
            c.scale = spec["scale"] ? spec["scale"].as<glm::vec2>() : glm::vec2{1.0f, 1.0f};
            e.add<Transform2DComponent>(c);
        }
        if (auto spec = entity["Transform3DComponent"]) {
            Transform3DComponent c;
            c.position = spec["position"] ? spec["position"].as<glm::vec3>() : glm::vec3{0.0f, 0.0f, 0.0f};
            c.orientation = spec["orientation"] ? spec["orientation"].as<glm::quat>() : glm::quat{1.0f, 0.0f, 0.0f, 0.0f};
            c.scale = spec["scale"] ? spec["scale"].as<glm::vec3>() : glm::vec3{1.0f, 1.0f, 1.0f};
            e.add<Transform3DComponent>(c);
        }
        if (auto spec = entity["ModelComponent"]) {
            ModelComponent c;
            c.mesh = spec["mesh"] ? spec["mesh"].as<std::string>() : "";
            c.material = spec["material"] ? spec["material"].as<std::string>() : "";
            e.add<ModelComponent>(c);
        }
        if (auto spec = entity["RigidBody3DComponent"]) {
            RigidBody3DComponent c;
            c.velocity = spec["velocity"] ? spec["velocity"].as<glm::vec3>() : glm::vec3{0.0f, 0.0f, 0.0f};
            c.gravity = spec["gravity"] ? spec["gravity"].as<bool>() : true;
            c.frozen = spec["frozen"] ? spec["frozen"].as<bool>() : false;
            c.bounciness = spec["bounciness"] ? spec["bounciness"].as<float>() : 0.5f;
            c.frictionCoefficient = spec["frictionCoefficient"] ? spec["frictionCoefficient"].as<float>() : 0.3f;
            c.rollingResistance = spec["rollingResistance"] ? spec["rollingResistance"].as<float>() : 0.0f;
            e.add<RigidBody3DComponent>(c);
        }
        if (auto spec = entity["BoxCollider3DComponent"]) {
            BoxCollider3DComponent c;
            c.position = spec["position"] ? spec["position"].as<glm::vec3>() : glm::vec3{0.0f, 0.0f, 0.0f};
            c.orientation = spec["orientation"] ? spec["orientation"].as<glm::quat>() : glm::quat{1.0f, 0.0f, 0.0f, 0.0f};
            c.mass = spec["mass"] ? spec["mass"].as<float>() : 1.0f;
            c.halfExtents = spec["halfExtents"] ? spec["halfExtents"].as<glm::vec3>() : glm::vec3{0.0f, 0.0f, 0.0f};
            c.applyScale = spec["applyScale"] ? spec["applyScale"].as<bool>() : true;
            e.add<BoxCollider3DComponent>(c);
        }
        if (auto spec = entity["SphereCollider3DComponent"]) {
            SphereCollider3DComponent c;
            c.position = spec["position"] ? spec["position"].as<glm::vec3>() : glm::vec3{0.0f, 0.0f, 0.0f};
            c.orientation = spec["orientation"] ? spec["orientation"].as<glm::quat>() : glm::quat{1.0f, 0.0f, 0.0f, 0.0f};
            c.mass = spec["mass"] ? spec["mass"].as<float>() : 1.0f;
            c.radius = spec["radius"] ? spec["radius"].as<float>() : 1.0f;
            e.add<SphereCollider3DComponent>(c);
        }
        if (auto spec = entity["CapsuleCollider3DComponent"]) {
            CapsuleCollider3DComponent c;
            c.position = spec["position"] ? spec["position"].as<glm::vec3>() : glm::vec3{0.0f, 0.0f, 0.0f};
            c.orientation = spec["orientation"] ? spec["orientation"].as<glm::quat>() : glm::quat{1.0f, 0.0f, 0.0f, 0.0f};
            c.mass = spec["mass"] ? spec["mass"].as<float>() : 1.0f;
            c.radius = spec["radius"] ? spec["radius"].as<float>() : 1.0f;
            c.height = spec["height"] ? spec["height"].as<float>() : 1.0f;
            e.add<CapsuleCollider3DComponent>(c);
        }
        if (auto spec = entity["ScriptComponent"]) {
            ScriptComponent c;
            c.script = spec["script"] ? spec["script"].as<std::string>() : "";
            c.active = spec["active"] ? spec["active"].as<bool>() : true;
            e.add<ScriptComponent>(c);
        }
        if (auto spec = entity["Camera3DComponent"]) {
            Camera3DComponent c;
            c.fov = spec["fov"] ? spec["fov"].as<float>() : 70.0f;
            c.pitch = spec["pitch"] ? spec["pitch"].as<float>() : 0.0f;
            e.add<Camera3DComponent>(c);
        }
        if (auto spec = entity["SelectComponent"]) {
            SelectComponent c;
            e.add<SelectComponent>(c);
        }
        if (auto spec = entity["PathComponent"]) {
            PathComponent c;
            c.speed = spec["speed"] ? spec["speed"].as<float>() : 0.0f;
            e.add<PathComponent>(c);
        }
        if (auto spec = entity["GridComponent"]) {
            GridComponent c;
            c.x = spec["x"] ? spec["x"].as<int>() : 0;
            c.z = spec["z"] ? spec["z"].as<int>() : 0;
            e.add<GridComponent>(c);
        }
        if (auto spec = entity["LightComponent"]) {
            LightComponent c;
            c.colour = spec["colour"] ? spec["colour"].as<glm::vec3>() : glm::vec3{1.0f, 1.0f, 1.0f};
            c.brightness = spec["brightness"] ? spec["brightness"].as<float>() : 1.0f;
            e.add<LightComponent>(c);
        }
        if (auto spec = entity["SunComponent"]) {
            SunComponent c;
            c.colour = spec["colour"] ? spec["colour"].as<glm::vec3>() : glm::vec3{1.0f, 1.0f, 1.0f};
            c.brightness = spec["brightness"] ? spec["brightness"].as<float>() : 1.0f;
            c.direction = spec["direction"] ? spec["direction"].as<glm::vec3>() : glm::vec3{0.0f, -1.0f, 0.0f};
            c.shadows = spec["shadows"] ? spec["shadows"].as<bool>() : false;
            e.add<SunComponent>(c);
        }
        if (auto spec = entity["AmbienceComponent"]) {
            AmbienceComponent c;
            c.colour = spec["colour"] ? spec["colour"].as<glm::vec3>() : glm::vec3{1.0f, 1.0f, 1.0f};
            c.brightness = spec["brightness"] ? spec["brightness"].as<float>() : 1.0f;
            e.add<AmbienceComponent>(c);
        }
        if (auto spec = entity["ParticleComponent"]) {
            ParticleComponent c;
            c.interval = spec["interval"] ? spec["interval"].as<float>() : 1.0f;
            c.velocity = spec["velocity"] ? spec["velocity"].as<glm::vec3>() : glm::vec3{0.0f, 0.0f, 0.0f};
            c.velocityNoise = spec["velocityNoise"] ? spec["velocityNoise"].as<float>() : 0.0f;
            c.acceleration = spec["acceleration"] ? spec["acceleration"].as<glm::vec3>() : glm::vec3{0.0f, -9.81f, 0.0f};
            c.scale = spec["scale"] ? spec["scale"].as<glm::vec3>() : glm::vec3{1.0f, 1.0f, 1.0f};
            c.life = spec["life"] ? spec["life"].as<float>() : 1.0f;
            e.add<ParticleComponent>(c);
        }
        if (auto spec = entity["MeshAnimationComponent"]) {
            MeshAnimationComponent c;
            c.name = spec["name"] ? spec["name"].as<std::string>() : "";
            c.time = spec["time"] ? spec["time"].as<float>() : 0.0f;
            c.speed = spec["speed"] ? spec["speed"].as<float>() : 1.0f;
            e.add<MeshAnimationComponent>(c);
        }
    }
}

ecs::Entity Copy(ecs::Registry* reg, ecs::Entity entity)
{
    ecs::Entity e = reg->New();
    if (entity.has<TemporaryComponent>()) {
        e.add<TemporaryComponent>(entity.get<TemporaryComponent>());
    }
    if (entity.has<NameComponent>()) {
        e.add<NameComponent>(entity.get<NameComponent>());
    }
    if (entity.has<Transform2DComponent>()) {
        e.add<Transform2DComponent>(entity.get<Transform2DComponent>());
    }
    if (entity.has<Transform3DComponent>()) {
        e.add<Transform3DComponent>(entity.get<Transform3DComponent>());
    }
    if (entity.has<ModelComponent>()) {
        e.add<ModelComponent>(entity.get<ModelComponent>());
    }
    if (entity.has<RigidBody3DComponent>()) {
        e.add<RigidBody3DComponent>(entity.get<RigidBody3DComponent>());
    }
    if (entity.has<BoxCollider3DComponent>()) {
        e.add<BoxCollider3DComponent>(entity.get<BoxCollider3DComponent>());
    }
    if (entity.has<SphereCollider3DComponent>()) {
        e.add<SphereCollider3DComponent>(entity.get<SphereCollider3DComponent>());
    }
    if (entity.has<CapsuleCollider3DComponent>()) {
        e.add<CapsuleCollider3DComponent>(entity.get<CapsuleCollider3DComponent>());
    }
    if (entity.has<ScriptComponent>()) {
        e.add<ScriptComponent>(entity.get<ScriptComponent>());
    }
    if (entity.has<Camera3DComponent>()) {
        e.add<Camera3DComponent>(entity.get<Camera3DComponent>());
    }
    if (entity.has<SelectComponent>()) {
        e.add<SelectComponent>(entity.get<SelectComponent>());
    }
    if (entity.has<PathComponent>()) {
        e.add<PathComponent>(entity.get<PathComponent>());
    }
    if (entity.has<GridComponent>()) {
        e.add<GridComponent>(entity.get<GridComponent>());
    }
    if (entity.has<LightComponent>()) {
        e.add<LightComponent>(entity.get<LightComponent>());
    }
    if (entity.has<SunComponent>()) {
        e.add<SunComponent>(entity.get<SunComponent>());
    }
    if (entity.has<AmbienceComponent>()) {
        e.add<AmbienceComponent>(entity.get<AmbienceComponent>());
    }
    if (entity.has<ParticleComponent>()) {
        e.add<ParticleComponent>(entity.get<ParticleComponent>());
    }
    if (entity.has<MeshAnimationComponent>()) {
        e.add<MeshAnimationComponent>(entity.get<MeshAnimationComponent>());
    }
    return e;
}

void Copy(ecs::Registry* source, ecs::Registry* target)
{
    for (auto entity : source->Each()) {
        Copy(target, entity);
    }
}

}
}
