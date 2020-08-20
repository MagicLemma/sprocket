#include "Serialiser.h"
#include "Log.h"
#include "Components.h"
#include "Maths.h"
#include "Yaml.h"

#include <yaml-cpp/yaml.h>
#include <fstream>

namespace Sprocket {

void Serialiser::Serialise(const std::string& file)
{
    YAML::Emitter out;
    out << YAML::BeginMap;
    out << YAML::Key << "Entities" << YAML::BeginSeq;
    d_scene->All([&](Entity& entity) {
        if (entity.Has<TemporaryComponent>()) { return; }

        out << YAML::BeginMap;
        if (entity.Has<NameComponent>()) {
            const auto& name = entity.Get<NameComponent>();
            out << YAML::Key << "NameComponent" << YAML::BeginMap;
            out << YAML::Key << "name" << YAML::Value << name.name;
            out << YAML::EndMap;
        }
        if (entity.Has<TransformComponent>()) {
            const auto& transform = entity.Get<TransformComponent>();
            out << YAML::Key << "TransformComponent" << YAML::BeginMap;
            out << YAML::Key << "position" << YAML::Value << transform.position;
            out << YAML::Key << "orientation" << YAML::Value << transform.orientation;
            out << YAML::EndMap;
        }
        if (entity.Has<ModelComponent>()) {
            const auto& model = entity.Get<ModelComponent>();
            out << YAML::Key << "ModelComponent" << YAML::BeginMap;
            out << YAML::Key << "model" << YAML::Value << model.model;
            out << YAML::Key << "scale" << YAML::Value << model.scale;
            out << YAML::Key << "texture" << YAML::Value << model.texture;
            out << YAML::Key << "shineDamper" << YAML::Value << model.shineDamper;
            out << YAML::Key << "reflectivity" << YAML::Value << model.reflectivity;
            out << YAML::EndMap;
        }
        if (entity.Has<PhysicsComponent>()) {
            const auto& physics = entity.Get<PhysicsComponent>();
            out << YAML::Key << "PhysicsComponents" << YAML::BeginMap;
            out << YAML::Key << "velocity" << YAML::Value << physics.velocity;
            out << YAML::Key << "gravity" << YAML::Value << physics.gravity;
            out << YAML::Key << "frozen" << YAML::Value << physics.frozen;
            out << YAML::Key << "collider" << YAML::Value << static_cast<int>(physics.collider);
            out << YAML::Key << "halfExtents" << YAML::Value << physics.halfExtents;
            out << YAML::Key << "radius" << YAML::Value << physics.radius;
            out << YAML::Key << "height" << YAML::Value << physics.height;
            out << YAML::Key << "mass" << YAML::Value << physics.mass;
            out << YAML::Key << "bounciness" << YAML::Value << physics.bounciness;
            out << YAML::Key << "frictionCoefficient" << YAML::Value << physics.frictionCoefficient;
            out << YAML::Key << "rollingResistance" << YAML::Value << physics.rollingResistance;
            out << YAML::EndMap;
        }
        if (entity.Has<ScriptComponent>()) {
            const auto& script = entity.Get<ScriptComponent>();
            out << YAML::Key << "ScriptComponent" << YAML::BeginMap;
            out << YAML::Key << "script" << YAML::Value << script.script;
            out << YAML::Key << "active" << YAML::Value << script.active;
            out << YAML::EndMap;
        }
        if (entity.Has<CameraComponent>()) {
            const auto& camera = entity.Get<CameraComponent>();
            out << YAML::Key << "CameraComponent" << YAML::BeginMap;
            out << YAML::Key << "fov" << YAML::Value << camera.fov;
            out << YAML::Key << "pitch" << YAML::Value << camera.pitch;
            out << YAML::EndMap;
        }
        if (entity.Has<SelectComponent>()) {
            const auto& select = entity.Get<SelectComponent>();
            out << YAML::Key << "SelectComponent" << true;
        }
        if (entity.Has<PathComponent>()) {
            const auto& path = entity.Get<PathComponent>();
            out << YAML::Key << "PathComponent" << YAML::BeginMap;
            out << YAML::Key << "speed" << YAML::Value << path.speed;
            out << YAML::EndMap;
        }
        if (entity.Has<GridComponent>()) {
            const auto& grid = entity.Get<GridComponent>();
            out << YAML::Key << "GridComponent" << YAML::BeginMap;
            out << YAML::Key << "x" << YAML::Value << grid.x;
            out << YAML::Key << "z" << YAML::Value << grid.z;
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

        if (auto name = entity["NameComponent"]) {
            NameComponent nc;
            nc.name = name["name"].as<std::string>();
            e.Add(nc);
        }

        if (auto transform = entity["TransformComponent"]) {
            TransformComponent tc;
            tc.position = transform["position"].as<Maths::vec3>();
            tc.orientation = transform["orientation"].as<Maths::quat>();
            e.Add(tc);
        }

        if (auto model = entity["ModelComponent"]) {
            ModelComponent mc;
            mc.model = model["model"].as<std::string>();
            mc.scale = model["scale"].as<float>();
            mc.texture = model["texture"].as<std::string>();
            mc.shineDamper = model["shineDamper"].as<float>();
            mc.reflectivity = model["reflectivity"].as<float>();
            e.Add(mc);
        }

        if (auto physics = entity["PhysicsComponent"]) {
            PhysicsComponent pc;
            pc.velocity = physics["velocity"].as<Maths::vec3>();
            pc.gravity = physics["gravity"].as<bool>();
            pc.frozen = physics["frozen"].as<bool>();
            pc.collider = static_cast<Collider>(physics["collider"].as<int>());
            pc.halfExtents = physics["halfExtents"].as<Maths::vec3>();
            pc.radius = physics["radius"].as<float>();
            pc.height = physics["height"].as<float>();
            pc.mass = physics["mass"].as<float>();
            pc.bounciness = physics["bounciness"].as<float>();
            pc.frictionCoefficient = physics["frictionCoefficient"].as<float>();
            pc.rollingResistance = physics["rollingResistance"].as<float>();
            e.Add(pc);
        }

        if (auto camera = entity["CameraComponent"]) {
            CameraComponent cc;
            cc.pitch = camera["fov"].as<float>();
            cc.pitch = camera["pitch"].as<float>();
            e.Add(cc);
        }

        if (auto select = entity["SelectComponent"]) {
            SelectComponent sc;
            e.Add(sc);
        }

        if (auto path = entity["PathComponent"]) {
            PathComponent pc;
            pc.speed = path["speed"].as<float>();
            e.Add(pc);
        }

        if (auto grid = entity["GridComponent"]) {
            GridComponent gc;
            gc.x = grid["x"].as<int>();
            gc.z = grid["z"].as<int>();
            e.Add(gc);
        }

        if (auto script = entity["ScriptComponent"]) {
            ScriptComponent sc;
            sc.script = script["script"].as<std::string>();
            sc.active = script["active"].as<bool>();
            e.Add(sc);
        }
    }
}

}