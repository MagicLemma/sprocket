// GENERATED FILE @ 2020-08-26 13:16:17.923701
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
    out << YAML::Key << "Entities" << YAML::BeginSeq;
out << YAML::Key << "Version" << YAML::Value << 1;
    scene->All([&](Entity& entity) {
        if (entity.Has<TemporaryComponent>()) { return; }

        out << YAML::BeginMap;
        if(entity.Has<TemporaryComponent>()) {
            const auto& c = entity.Get<TemporaryComponent>();
            out << YAML::Key << "TemporaryComponent" << YAML::BeginMap;
            out << YAML::EndMap;
        }
        if(entity.Has<NameComponent>()) {
            const auto& c = entity.Get<NameComponent>();
            out << YAML::Key << "NameComponent" << YAML::BeginMap;
            out << YAML::Key << "name" << YAML::Value << c.name;
            out << YAML::EndMap;
        }
        if(entity.Has<TransformComponent>()) {
            const auto& c = entity.Get<TransformComponent>();
            out << YAML::Key << "TransformComponent" << YAML::BeginMap;
            out << YAML::Key << "position" << YAML::Value << c.position;
            out << YAML::Key << "orientation" << YAML::Value << c.orientation;
            out << YAML::EndMap;
        }
        if(entity.Has<ModelComponent>()) {
            const auto& c = entity.Get<ModelComponent>();
            out << YAML::Key << "ModelComponent" << YAML::BeginMap;
            out << YAML::Key << "model" << YAML::Value << c.model;
            out << YAML::Key << "scale" << YAML::Value << c.scale;
            out << YAML::Key << "texture" << YAML::Value << c.texture;
            out << YAML::Key << "shineDamper" << YAML::Value << c.shineDamper;
            out << YAML::Key << "reflectivity" << YAML::Value << c.reflectivity;
            out << YAML::EndMap;
        }
        if(entity.Has<RigidBody3DComponent>()) {
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
        if(entity.Has<BoxCollider3DComponent>()) {
            const auto& c = entity.Get<BoxCollider3DComponent>();
            out << YAML::Key << "BoxCollider3DComponent" << YAML::BeginMap;
            out << YAML::Key << "mass" << YAML::Value << c.mass;
            out << YAML::Key << "halfExtents" << YAML::Value << c.halfExtents;
            out << YAML::EndMap;
        }
        if(entity.Has<SphereCollider3DComponent>()) {
            const auto& c = entity.Get<SphereCollider3DComponent>();
            out << YAML::Key << "SphereCollider3DComponent" << YAML::BeginMap;
            out << YAML::Key << "mass" << YAML::Value << c.mass;
            out << YAML::Key << "radius" << YAML::Value << c.radius;
            out << YAML::EndMap;
        }
        if(entity.Has<CapsuleCollider3DComponent>()) {
            const auto& c = entity.Get<CapsuleCollider3DComponent>();
            out << YAML::Key << "CapsuleCollider3DComponent" << YAML::BeginMap;
            out << YAML::Key << "mass" << YAML::Value << c.mass;
            out << YAML::Key << "radius" << YAML::Value << c.radius;
            out << YAML::Key << "height" << YAML::Value << c.height;
            out << YAML::EndMap;
        }
        if(entity.Has<ScriptComponent>()) {
            const auto& c = entity.Get<ScriptComponent>();
            out << YAML::Key << "ScriptComponent" << YAML::BeginMap;
            out << YAML::Key << "script" << YAML::Value << c.script;
            out << YAML::Key << "active" << YAML::Value << c.active;
            out << YAML::EndMap;
        }
        if(entity.Has<CameraComponent>()) {
            const auto& c = entity.Get<CameraComponent>();
            out << YAML::Key << "CameraComponent" << YAML::BeginMap;
            out << YAML::Key << "fov" << YAML::Value << c.fov;
            out << YAML::Key << "pitch" << YAML::Value << c.pitch;
            out << YAML::EndMap;
        }
        if(entity.Has<SelectComponent>()) {
            const auto& c = entity.Get<SelectComponent>();
            out << YAML::Key << "SelectComponent" << YAML::BeginMap;
            out << YAML::EndMap;
        }
        if(entity.Has<PathComponent>()) {
            const auto& c = entity.Get<PathComponent>();
            out << YAML::Key << "PathComponent" << YAML::BeginMap;
            out << YAML::Key << "speed" << YAML::Value << c.speed;
            out << YAML::EndMap;
        }
        if(entity.Has<GridComponent>()) {
            const auto& c = entity.Get<GridComponent>();
            out << YAML::Key << "GridComponent" << YAML::BeginMap;
            out << YAML::Key << "x" << YAML::Value << c.x;
            out << YAML::Key << "z" << YAML::Value << c.z;
            out << YAML::EndMap;
        }
        if(entity.Has<LightComponent>()) {
            const auto& c = entity.Get<LightComponent>();
            out << YAML::Key << "LightComponent" << YAML::BeginMap;
            out << YAML::Key << "colour" << YAML::Value << c.colour;
            out << YAML::Key << "attenuation" << YAML::Value << c.attenuation;
            out << YAML::Key << "brightness" << YAML::Value << c.brightness;
            out << YAML::EndMap;
        }

        out << YAML::EndMap;
    });
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
        Entity e = scene->NewEntity();
        if (auto spec = entity["TemporaryComponent"]) {
            TemporaryComponent c;
            e.Add(c);
        }
        if (auto spec = entity["NameComponent"]) {
            NameComponent c;
            c.name = spec["name"].as<std::string>();
            e.Add(c);
        }
        if (auto spec = entity["TransformComponent"]) {
            TransformComponent c;
            c.position = spec["position"].as<Maths::vec3>();
            c.orientation = spec["orientation"].as<Maths::quat>();
            e.Add(c);
        }
        if (auto spec = entity["ModelComponent"]) {
            ModelComponent c;
            c.model = spec["model"].as<std::string>();
            c.scale = spec["scale"].as<float>();
            c.texture = spec["texture"].as<std::string>();
            c.shineDamper = spec["shineDamper"].as<float>();
            c.reflectivity = spec["reflectivity"].as<float>();
            e.Add(c);
        }
        if (auto spec = entity["RigidBody3DComponent"]) {
            RigidBody3DComponent c;
            c.velocity = spec["velocity"].as<Maths::vec3>();
            c.gravity = spec["gravity"].as<bool>();
            c.frozen = spec["frozen"].as<bool>();
            c.bounciness = spec["bounciness"].as<float>();
            c.frictionCoefficient = spec["frictionCoefficient"].as<float>();
            c.rollingResistance = spec["rollingResistance"].as<float>();
            e.Add(c);
        }
        if (auto spec = entity["BoxCollider3DComponent"]) {
            BoxCollider3DComponent c;
            c.mass = spec["mass"].as<float>();
            c.halfExtents = spec["halfExtents"].as<Maths::vec3>();
            e.Add(c);
        }
        if (auto spec = entity["SphereCollider3DComponent"]) {
            SphereCollider3DComponent c;
            c.mass = spec["mass"].as<float>();
            c.radius = spec["radius"].as<float>();
            e.Add(c);
        }
        if (auto spec = entity["CapsuleCollider3DComponent"]) {
            CapsuleCollider3DComponent c;
            c.mass = spec["mass"].as<float>();
            c.radius = spec["radius"].as<float>();
            c.height = spec["height"].as<float>();
            e.Add(c);
        }
        if (auto spec = entity["ScriptComponent"]) {
            ScriptComponent c;
            c.script = spec["script"].as<std::string>();
            c.active = spec["active"].as<bool>();
            e.Add(c);
        }
        if (auto spec = entity["CameraComponent"]) {
            CameraComponent c;
            c.fov = spec["fov"].as<float>();
            c.pitch = spec["pitch"].as<float>();
            e.Add(c);
        }
        if (auto spec = entity["SelectComponent"]) {
            SelectComponent c;
            e.Add(c);
        }
        if (auto spec = entity["PathComponent"]) {
            PathComponent c;
            c.speed = spec["speed"].as<float>();
            e.Add(c);
        }
        if (auto spec = entity["GridComponent"]) {
            GridComponent c;
            c.x = spec["x"].as<int>();
            c.z = spec["z"].as<int>();
            e.Add(c);
        }
        if (auto spec = entity["LightComponent"]) {
            LightComponent c;
            c.colour = spec["colour"].as<Maths::vec3>();
            c.attenuation = spec["attenuation"].as<Maths::vec3>();
            c.brightness = spec["brightness"].as<float>();
            e.Add(c);
        }

    }
}

void Copy(std::shared_ptr<Scene> source, std::shared_ptr<Scene> target)
{
    target->Clear();

    source->All([&](Entity& entity) {
        Entity e = target->NewEntity();
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

    });
}

}
}
