// GENERATED FILE
#include "Loader.h"
#include "Log.h"
#include "Components.h"
#include "Maths.h"
#include "Yaml.h"
#include "Scene.h"
#include "Updater.h"

#include <yaml-cpp/yaml.h>
#include <fstream>
#include <memory>

namespace Sprocket {
namespace Loader {

void Save(const std::string& file, std::shared_ptr<Scene> scene)
{
    YAML::Emitter out;
    out << YAML::BeginMap;
    out << YAML::Key << "Version" << YAML::Value << 2;

    out << YAML::Key << "Entities" << YAML::BeginSeq;
    for (auto entity : scene->Reg()->Fast()) {
        if (entity.Has<TemporaryComponent>()) { return; }
        out << YAML::BeginMap;
        if (entity.Has<TemporaryComponent>()) {
            const auto& c = entity.Get<TemporaryComponent>();
            out << YAML::Key << "TemporaryComponent" << YAML::BeginMap;
            out << YAML::EndMap;
        }
        if (entity.Has<NameComponent>()) {
            const auto& c = entity.Get<NameComponent>();
            out << YAML::Key << "NameComponent" << YAML::BeginMap;
            out << YAML::Key << "name" << YAML::Value << c.name;
            out << YAML::EndMap;
        }
        if (entity.Has<TransformComponent>()) {
            const auto& c = entity.Get<TransformComponent>();
            out << YAML::Key << "TransformComponent" << YAML::BeginMap;
            out << YAML::Key << "position" << YAML::Value << c.position;
            out << YAML::Key << "orientation" << YAML::Value << c.orientation;
            out << YAML::Key << "scale" << YAML::Value << c.scale;
            out << YAML::EndMap;
        }
        if (entity.Has<ModelComponent>()) {
            const auto& c = entity.Get<ModelComponent>();
            out << YAML::Key << "ModelComponent" << YAML::BeginMap;
            out << YAML::Key << "mesh" << YAML::Value << c.mesh;
            out << YAML::Key << "material" << YAML::Value << c.material;
            out << YAML::EndMap;
        }
        if (entity.Has<RigidBody3DComponent>()) {
            const auto& c = entity.Get<RigidBody3DComponent>();
            out << YAML::Key << "RigidBody3DComponent" << YAML::BeginMap;
            out << YAML::Key << "velocity" << YAML::Value << c.velocity;
            out << YAML::Key << "gravity" << YAML::Value << c.gravity;
            out << YAML::Key << "frozen" << YAML::Value << c.frozen;
            out << YAML::Key << "bounciness" << YAML::Value << c.bounciness;
            out << YAML::Key << "frictionCoefficient" << YAML::Value << c.frictionCoefficient;
            out << YAML::Key << "rollingResistance" << YAML::Value << c.rollingResistance;
            out << YAML::EndMap;
        }
        if (entity.Has<BoxCollider3DComponent>()) {
            const auto& c = entity.Get<BoxCollider3DComponent>();
            out << YAML::Key << "BoxCollider3DComponent" << YAML::BeginMap;
            out << YAML::Key << "position" << YAML::Value << c.position;
            out << YAML::Key << "orientation" << YAML::Value << c.orientation;
            out << YAML::Key << "mass" << YAML::Value << c.mass;
            out << YAML::Key << "halfExtents" << YAML::Value << c.halfExtents;
            out << YAML::Key << "applyScale" << YAML::Value << c.applyScale;
            out << YAML::EndMap;
        }
        if (entity.Has<SphereCollider3DComponent>()) {
            const auto& c = entity.Get<SphereCollider3DComponent>();
            out << YAML::Key << "SphereCollider3DComponent" << YAML::BeginMap;
            out << YAML::Key << "position" << YAML::Value << c.position;
            out << YAML::Key << "orientation" << YAML::Value << c.orientation;
            out << YAML::Key << "mass" << YAML::Value << c.mass;
            out << YAML::Key << "radius" << YAML::Value << c.radius;
            out << YAML::EndMap;
        }
        if (entity.Has<CapsuleCollider3DComponent>()) {
            const auto& c = entity.Get<CapsuleCollider3DComponent>();
            out << YAML::Key << "CapsuleCollider3DComponent" << YAML::BeginMap;
            out << YAML::Key << "position" << YAML::Value << c.position;
            out << YAML::Key << "orientation" << YAML::Value << c.orientation;
            out << YAML::Key << "mass" << YAML::Value << c.mass;
            out << YAML::Key << "radius" << YAML::Value << c.radius;
            out << YAML::Key << "height" << YAML::Value << c.height;
            out << YAML::EndMap;
        }
        if (entity.Has<ScriptComponent>()) {
            const auto& c = entity.Get<ScriptComponent>();
            out << YAML::Key << "ScriptComponent" << YAML::BeginMap;
            out << YAML::Key << "script" << YAML::Value << c.script;
            out << YAML::Key << "active" << YAML::Value << c.active;
            out << YAML::EndMap;
        }
        if (entity.Has<CameraComponent>()) {
            const auto& c = entity.Get<CameraComponent>();
            out << YAML::Key << "CameraComponent" << YAML::BeginMap;
            out << YAML::Key << "fov" << YAML::Value << c.fov;
            out << YAML::Key << "pitch" << YAML::Value << c.pitch;
            out << YAML::EndMap;
        }
        if (entity.Has<SelectComponent>()) {
            const auto& c = entity.Get<SelectComponent>();
            out << YAML::Key << "SelectComponent" << YAML::BeginMap;
            out << YAML::EndMap;
        }
        if (entity.Has<PathComponent>()) {
            const auto& c = entity.Get<PathComponent>();
            out << YAML::Key << "PathComponent" << YAML::BeginMap;
            out << YAML::Key << "speed" << YAML::Value << c.speed;
            out << YAML::EndMap;
        }
        if (entity.Has<GridComponent>()) {
            const auto& c = entity.Get<GridComponent>();
            out << YAML::Key << "GridComponent" << YAML::BeginMap;
            out << YAML::Key << "x" << YAML::Value << c.x;
            out << YAML::Key << "z" << YAML::Value << c.z;
            out << YAML::EndMap;
        }
        if (entity.Has<LightComponent>()) {
            const auto& c = entity.Get<LightComponent>();
            out << YAML::Key << "LightComponent" << YAML::BeginMap;
            out << YAML::Key << "colour" << YAML::Value << c.colour;
            out << YAML::Key << "brightness" << YAML::Value << c.brightness;
            out << YAML::EndMap;
        }
        if (entity.Has<SunComponent>()) {
            const auto& c = entity.Get<SunComponent>();
            out << YAML::Key << "SunComponent" << YAML::BeginMap;
            out << YAML::Key << "colour" << YAML::Value << c.colour;
            out << YAML::Key << "brightness" << YAML::Value << c.brightness;
            out << YAML::Key << "direction" << YAML::Value << c.direction;
            out << YAML::Key << "shadows" << YAML::Value << c.shadows;
            out << YAML::EndMap;
        }
        if (entity.Has<AmbienceComponent>()) {
            const auto& c = entity.Get<AmbienceComponent>();
            out << YAML::Key << "AmbienceComponent" << YAML::BeginMap;
            out << YAML::Key << "colour" << YAML::Value << c.colour;
            out << YAML::Key << "brightness" << YAML::Value << c.brightness;
            out << YAML::EndMap;
        }
        if (entity.Has<ParticleComponent>()) {
            const auto& c = entity.Get<ParticleComponent>();
            out << YAML::Key << "ParticleComponent" << YAML::BeginMap;
            out << YAML::Key << "interval" << YAML::Value << c.interval;
            out << YAML::Key << "velocity" << YAML::Value << c.velocity;
            out << YAML::Key << "velocityNoise" << YAML::Value << c.velocityNoise;
            out << YAML::Key << "acceleration" << YAML::Value << c.acceleration;
            out << YAML::Key << "scale" << YAML::Value << c.scale;
            out << YAML::Key << "life" << YAML::Value << c.life;
            out << YAML::EndMap;
        }
        if (entity.Has<AnimationComponent>()) {
            const auto& c = entity.Get<AnimationComponent>();
            out << YAML::Key << "AnimationComponent" << YAML::BeginMap;
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

void Load(const std::string& file, std::shared_ptr<Scene> scene)
{
    scene->Clear();

    std::ifstream stream(file);
    std::stringstream sstream;
    sstream << stream.rdbuf();

    YAML::Node data = YAML::Load(sstream.str());
    UpdateScene(data);

    if (!data["Entities"]) {
        return; // TODO: Error checking
    }

    auto entities = data["Entities"];
    for (auto entity : entities) {
        ECS::Entity e = scene->Reg()->New();
        if (auto spec = entity["TemporaryComponent"]) {
            TemporaryComponent c;
            e.Add<TemporaryComponent>(c);
        }
        if (auto spec = entity["NameComponent"]) {
            NameComponent c;
            c.name = spec["name"] ? spec["name"].as<std::string>() : "Entity";
            e.Add<NameComponent>(c);
        }
        if (auto spec = entity["TransformComponent"]) {
            TransformComponent c;
            c.position = spec["position"] ? spec["position"].as<glm::vec3>() : glm::vec3{0.0f, 0.0f, 0.0f};
            c.orientation = spec["orientation"] ? spec["orientation"].as<glm::quat>() : glm::quat{1.0f, 0.0f, 0.0f, 0.0f};
            c.scale = spec["scale"] ? spec["scale"].as<glm::vec3>() : glm::vec3{1.0f, 1.0f, 1.0f};
            e.Add<TransformComponent>(c);
        }
        if (auto spec = entity["ModelComponent"]) {
            ModelComponent c;
            c.mesh = spec["mesh"] ? spec["mesh"].as<std::string>() : "";
            c.material = spec["material"] ? spec["material"].as<std::string>() : "";
            e.Add<ModelComponent>(c);
        }
        if (auto spec = entity["RigidBody3DComponent"]) {
            RigidBody3DComponent c;
            c.velocity = spec["velocity"] ? spec["velocity"].as<glm::vec3>() : glm::vec3{0.0f, 0.0f, 0.0f};
            c.gravity = spec["gravity"] ? spec["gravity"].as<bool>() : true;
            c.frozen = spec["frozen"] ? spec["frozen"].as<bool>() : false;
            c.bounciness = spec["bounciness"] ? spec["bounciness"].as<float>() : 0.5f;
            c.frictionCoefficient = spec["frictionCoefficient"] ? spec["frictionCoefficient"].as<float>() : 0.3f;
            c.rollingResistance = spec["rollingResistance"] ? spec["rollingResistance"].as<float>() : 0.0f;
            e.Add<RigidBody3DComponent>(c);
        }
        if (auto spec = entity["BoxCollider3DComponent"]) {
            BoxCollider3DComponent c;
            c.position = spec["position"] ? spec["position"].as<glm::vec3>() : glm::vec3{0.0f, 0.0f, 0.0f};
            c.orientation = spec["orientation"] ? spec["orientation"].as<glm::quat>() : glm::quat{1.0f, 0.0f, 0.0f, 0.0f};
            c.mass = spec["mass"] ? spec["mass"].as<float>() : 1.0f;
            c.halfExtents = spec["halfExtents"] ? spec["halfExtents"].as<glm::vec3>() : glm::vec3{0.0f, 0.0f, 0.0f};
            c.applyScale = spec["applyScale"] ? spec["applyScale"].as<bool>() : true;
            e.Add<BoxCollider3DComponent>(c);
        }
        if (auto spec = entity["SphereCollider3DComponent"]) {
            SphereCollider3DComponent c;
            c.position = spec["position"] ? spec["position"].as<glm::vec3>() : glm::vec3{0.0f, 0.0f, 0.0f};
            c.orientation = spec["orientation"] ? spec["orientation"].as<glm::quat>() : glm::quat{1.0f, 0.0f, 0.0f, 0.0f};
            c.mass = spec["mass"] ? spec["mass"].as<float>() : 1.0f;
            c.radius = spec["radius"] ? spec["radius"].as<float>() : 1.0f;
            e.Add<SphereCollider3DComponent>(c);
        }
        if (auto spec = entity["CapsuleCollider3DComponent"]) {
            CapsuleCollider3DComponent c;
            c.position = spec["position"] ? spec["position"].as<glm::vec3>() : glm::vec3{0.0f, 0.0f, 0.0f};
            c.orientation = spec["orientation"] ? spec["orientation"].as<glm::quat>() : glm::quat{1.0f, 0.0f, 0.0f, 0.0f};
            c.mass = spec["mass"] ? spec["mass"].as<float>() : 1.0f;
            c.radius = spec["radius"] ? spec["radius"].as<float>() : 1.0f;
            c.height = spec["height"] ? spec["height"].as<float>() : 1.0f;
            e.Add<CapsuleCollider3DComponent>(c);
        }
        if (auto spec = entity["ScriptComponent"]) {
            ScriptComponent c;
            c.script = spec["script"] ? spec["script"].as<std::string>() : "";
            c.active = spec["active"] ? spec["active"].as<bool>() : true;
            e.Add<ScriptComponent>(c);
        }
        if (auto spec = entity["CameraComponent"]) {
            CameraComponent c;
            c.fov = spec["fov"] ? spec["fov"].as<float>() : 70.0f;
            c.pitch = spec["pitch"] ? spec["pitch"].as<float>() : 0.0f;
            e.Add<CameraComponent>(c);
        }
        if (auto spec = entity["SelectComponent"]) {
            SelectComponent c;
            e.Add<SelectComponent>(c);
        }
        if (auto spec = entity["PathComponent"]) {
            PathComponent c;
            c.speed = spec["speed"] ? spec["speed"].as<float>() : 0.0f;
            e.Add<PathComponent>(c);
        }
        if (auto spec = entity["GridComponent"]) {
            GridComponent c;
            c.x = spec["x"] ? spec["x"].as<int>() : 0;
            c.z = spec["z"] ? spec["z"].as<int>() : 0;
            e.Add<GridComponent>(c);
        }
        if (auto spec = entity["LightComponent"]) {
            LightComponent c;
            c.colour = spec["colour"] ? spec["colour"].as<glm::vec3>() : glm::vec3{1.0f, 1.0f, 1.0f};
            c.brightness = spec["brightness"] ? spec["brightness"].as<float>() : 1.0f;
            e.Add<LightComponent>(c);
        }
        if (auto spec = entity["SunComponent"]) {
            SunComponent c;
            c.colour = spec["colour"] ? spec["colour"].as<glm::vec3>() : glm::vec3{1.0f, 1.0f, 1.0f};
            c.brightness = spec["brightness"] ? spec["brightness"].as<float>() : 1.0f;
            c.direction = spec["direction"] ? spec["direction"].as<glm::vec3>() : glm::vec3{0.0f, -1.0f, 0.0f};
            c.shadows = spec["shadows"] ? spec["shadows"].as<bool>() : false;
            e.Add<SunComponent>(c);
        }
        if (auto spec = entity["AmbienceComponent"]) {
            AmbienceComponent c;
            c.colour = spec["colour"] ? spec["colour"].as<glm::vec3>() : glm::vec3{1.0f, 1.0f, 1.0f};
            c.brightness = spec["brightness"] ? spec["brightness"].as<float>() : 1.0f;
            e.Add<AmbienceComponent>(c);
        }
        if (auto spec = entity["ParticleComponent"]) {
            ParticleComponent c;
            c.interval = spec["interval"] ? spec["interval"].as<float>() : 1.0f;
            c.velocity = spec["velocity"] ? spec["velocity"].as<glm::vec3>() : glm::vec3{0.0f, 0.0f, 0.0f};
            c.velocityNoise = spec["velocityNoise"] ? spec["velocityNoise"].as<float>() : 0.0f;
            c.acceleration = spec["acceleration"] ? spec["acceleration"].as<glm::vec3>() : glm::vec3{0.0f, -9.81f, 0.0f};
            c.scale = spec["scale"] ? spec["scale"].as<glm::vec3>() : glm::vec3{1.0f, 1.0f, 1.0f};
            c.life = spec["life"] ? spec["life"].as<float>() : 1.0f;
            e.Add<ParticleComponent>(c);
        }
        if (auto spec = entity["AnimationComponent"]) {
            AnimationComponent c;
            c.name = spec["name"] ? spec["name"].as<std::string>() : "";
            c.time = spec["time"] ? spec["time"].as<float>() : 0.0f;
            c.speed = spec["speed"] ? spec["speed"].as<float>() : 1.0f;
            e.Add<AnimationComponent>(c);
        }
    }
}

ECS::Entity Copy(std::shared_ptr<Scene> scene, ECS::Entity entity)
{
    ECS::Entity e = scene->Reg()->New();
    if (entity.Has<TemporaryComponent>()) {
        e.Add<TemporaryComponent>(entity.Get<TemporaryComponent>());
    }
    if (entity.Has<NameComponent>()) {
        e.Add<NameComponent>(entity.Get<NameComponent>());
    }
    if (entity.Has<TransformComponent>()) {
        e.Add<TransformComponent>(entity.Get<TransformComponent>());
    }
    if (entity.Has<ModelComponent>()) {
        e.Add<ModelComponent>(entity.Get<ModelComponent>());
    }
    if (entity.Has<RigidBody3DComponent>()) {
        e.Add<RigidBody3DComponent>(entity.Get<RigidBody3DComponent>());
    }
    if (entity.Has<BoxCollider3DComponent>()) {
        e.Add<BoxCollider3DComponent>(entity.Get<BoxCollider3DComponent>());
    }
    if (entity.Has<SphereCollider3DComponent>()) {
        e.Add<SphereCollider3DComponent>(entity.Get<SphereCollider3DComponent>());
    }
    if (entity.Has<CapsuleCollider3DComponent>()) {
        e.Add<CapsuleCollider3DComponent>(entity.Get<CapsuleCollider3DComponent>());
    }
    if (entity.Has<ScriptComponent>()) {
        e.Add<ScriptComponent>(entity.Get<ScriptComponent>());
    }
    if (entity.Has<CameraComponent>()) {
        e.Add<CameraComponent>(entity.Get<CameraComponent>());
    }
    if (entity.Has<SelectComponent>()) {
        e.Add<SelectComponent>(entity.Get<SelectComponent>());
    }
    if (entity.Has<PathComponent>()) {
        e.Add<PathComponent>(entity.Get<PathComponent>());
    }
    if (entity.Has<GridComponent>()) {
        e.Add<GridComponent>(entity.Get<GridComponent>());
    }
    if (entity.Has<LightComponent>()) {
        e.Add<LightComponent>(entity.Get<LightComponent>());
    }
    if (entity.Has<SunComponent>()) {
        e.Add<SunComponent>(entity.Get<SunComponent>());
    }
    if (entity.Has<AmbienceComponent>()) {
        e.Add<AmbienceComponent>(entity.Get<AmbienceComponent>());
    }
    if (entity.Has<ParticleComponent>()) {
        e.Add<ParticleComponent>(entity.Get<ParticleComponent>());
    }
    if (entity.Has<AnimationComponent>()) {
        e.Add<AnimationComponent>(entity.Get<AnimationComponent>());
    }
    return e;
}

void Copy(std::shared_ptr<Scene> source, std::shared_ptr<Scene> target)
{
    target->Clear();
    for (auto entity : source->Reg()->Fast()) {
        Copy(target, entity);
    }
}

}
}
