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
    out << YAML::Key << "Scene";
    out << YAML::Value << "Scene Name";

    out << YAML::Key << "Entities" << YAML::BeginSeq;
    d_scene->All([&](Entity& entity) {
        if (entity.Has<TemporaryComponent>()) { return; }

        out << YAML::BeginMap;
        if (entity.Has<NameComponent>()) {
            const auto& name = entity.Get<NameComponent>();
            out << YAML::Key << "Name" << YAML::BeginMap;
            out << YAML::Key << "Name" << YAML::Value << name.name;
            out << YAML::EndMap;
        }
        if (entity.Has<TransformComponent>()) {
            const auto& transform = entity.Get<TransformComponent>();
            out << YAML::Key << "Transform" << YAML::BeginMap;
            out << YAML::Key << "Position" << YAML::Value << transform.position;
            out << YAML::Key << "Orientation" << YAML::Value << transform.orientation;
            out << YAML::EndMap;
        }
        if (entity.Has<ModelComponent>()) {
            const auto& model = entity.Get<ModelComponent>();
            out << YAML::Key << "Model" << YAML::BeginMap;
            out << YAML::Key << "Model" << YAML::Value << model.model;
            out << YAML::Key << "Scale" << YAML::Value << model.scale;
            out << YAML::Key << "Texture" << YAML::Value << model.texture;
            out << YAML::Key << "ShineDamper" << YAML::Value << model.shineDamper;
            out << YAML::Key << "Reflectivity" << YAML::Value << model.reflectivity;
            out << YAML::EndMap;
        }
        if (entity.Has<PhysicsComponent>()) {
            const auto& physics = entity.Get<PhysicsComponent>();
            out << YAML::Key << "Physics" << YAML::BeginMap;
            out << YAML::Key << "Velocity" << YAML::Value << physics.velocity;
            out << YAML::Key << "Gravity" << YAML::Value << physics.gravity;
            out << YAML::Key << "Frozen" << YAML::Value << physics.frozen;
            out << YAML::Key << "Collider" << YAML::Value << static_cast<int>(physics.collider);
            out << YAML::Key << "HalfExtents" << YAML::Value << physics.halfExtents;
            out << YAML::Key << "Radius" << YAML::Value << physics.radius;
            out << YAML::Key << "Height" << YAML::Value << physics.height;
            out << YAML::Key << "Mass" << YAML::Value << physics.mass;
            out << YAML::Key << "Bounciness" << YAML::Value << physics.bounciness;
            out << YAML::Key << "FrictionCoefficient" << YAML::Value << physics.frictionCoefficient;
            out << YAML::Key << "RollingResistance" << YAML::Value << physics.rollingResistance;
            out << YAML::EndMap;
        }
        if (entity.Has<ScriptComponent>()) {
            const auto& script = entity.Get<ScriptComponent>();
            out << YAML::Key << "Script" << YAML::BeginMap;
            out << YAML::Key << "Script" << YAML::Value << script.script;
            out << YAML::Key << "Active" << YAML::Value << script.active;
            out << YAML::EndMap;
        }
        if (entity.Has<CameraComponent>()) {
            const auto& camera = entity.Get<CameraComponent>();
            out << YAML::Key << "Camera" << YAML::BeginMap;
            out << YAML::Key << "FOV" << YAML::Value << camera.fov;
            out << YAML::Key << "Pitch" << YAML::Value << camera.pitch;
            out << YAML::EndMap;
        }
        if (entity.Has<SelectComponent>()) {
            const auto& select = entity.Get<SelectComponent>();
            // No need to store the values.
            out << YAML::Key << "Select" << true;
        }
        if (entity.Has<PathComponent>()) {
            const auto& path = entity.Get<PathComponent>();
            out << YAML::Key << "Path" << YAML::BeginMap;
            //out << YAML::Key << "Markers" << YAML::Value << path.markers; TODO - Fix this
            out << YAML::Key << "Speed" << YAML::Value << path.speed;
            out << YAML::EndMap;
        }
        if (entity.Has<GridComponent>()) {
            const auto& grid = entity.Get<GridComponent>();
            out << YAML::Key << "Grid" << YAML::BeginMap;
            out << YAML::Key << "X" << YAML::Value << grid.x;
            out << YAML::Key << "Z" << YAML::Value << grid.z;
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

        if (auto name = entity["Name"]) {
            NameComponent nc;
            nc.name = name["Name"].as<std::string>();
            e.Add(nc);
        }

        if (auto transform = entity["Transform"]) {
            TransformComponent tc;
            tc.position = transform["Position"].as<Maths::vec3>();
            tc.orientation = transform["Orientation"].as<Maths::quat>();
            e.Add(tc);
        }

        if (auto model = entity["Model"]) {
            ModelComponent mc;
            mc.model = model["Model"].as<std::string>();
            mc.scale = model["Scale"].as<float>();
            mc.texture = model["Texture"].as<std::string>();
            mc.shineDamper = model["ShineDamper"].as<float>();
            mc.reflectivity = model["Reflectivity"].as<float>();
            e.Add(mc);
        }

        if (auto physics = entity["Physics"]) {
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

        if (auto camera = entity["Camera"]) {
            CameraComponent cc;
            // Projection does not need to be stored
            cc.pitch = camera["FOV"].as<float>();
            cc.pitch = camera["Pitch"].as<float>();
            e.Add(cc);
        }

        if (auto select = entity["Select"]) {
            SelectComponent sc;
            e.Add(sc);
        }

        if (auto path = entity["Path"]) {
            PathComponent pc; // TODO Sort this
            //pc.markers = path["Markers"].as<std::queue<Maths::vec3>>();
            pc.speed = path["Speed"].as<float>();
            e.Add(pc);
        }

        if (auto grid = entity["Grid"]) {
            GridComponent gc;
            gc.x = grid["X"].as<int>();
            gc.z = grid["Z"].as<int>();
            e.Add(gc);
        }

        // This needs to get added last as scripts may access other components.
        if (auto script = entity["Script"]) {
            ScriptComponent sc;
            sc.script = script["Script"].as<std::string>();
            sc.active = script["Active"].as<bool>();
            e.Add(sc);
        }
    }
}

}