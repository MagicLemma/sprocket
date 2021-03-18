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
    for (auto entity : reg->all()) {
        if (entity.has<TemporaryComponent>()) { return; }
        out << YAML::BeginMap;
        out << YAML::Key << "ID#" << YAML::Value << entity.id();
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
    for (ecs::Entity e : reg->all()) {
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

    // Performs any extra transformations to values that cannot be done during
    // yaml decoding, for example converting entity IDs to their new values.
    const auto transform = [&](auto&& param) {
        if constexpr(std::is_same_v<decltype(param), ecs::Identifier>) {
            return param;
        }
        else {
            return param;
        }
    };

    auto entities = data["Entities"];

    for (auto entity : entities) {

    }

    for (auto entity : entities) {
        ecs::Entity e = reg->create();
        if (auto spec = entity["TemporaryComponent"]) {
            TemporaryComponent c;
            e.add<TemporaryComponent>(c);
        }
        if (auto spec = entity["NameComponent"]) {
            NameComponent c;
            c.name = transform(spec["name"].as<std::string>());
            e.add<NameComponent>(c);
        }
        if (auto spec = entity["Transform2DComponent"]) {
            Transform2DComponent c;
            c.position = transform(spec["position"].as<glm::vec2>());
            c.rotation = transform(spec["rotation"].as<float>());
            c.scale = transform(spec["scale"].as<glm::vec2>());
            e.add<Transform2DComponent>(c);
        }
        if (auto spec = entity["Transform3DComponent"]) {
            Transform3DComponent c;
            c.position = transform(spec["position"].as<glm::vec3>());
            c.orientation = transform(spec["orientation"].as<glm::quat>());
            c.scale = transform(spec["scale"].as<glm::vec3>());
            e.add<Transform3DComponent>(c);
        }
        if (auto spec = entity["ModelComponent"]) {
            ModelComponent c;
            c.mesh = transform(spec["mesh"].as<std::string>());
            c.material = transform(spec["material"].as<std::string>());
            e.add<ModelComponent>(c);
        }
        if (auto spec = entity["RigidBody3DComponent"]) {
            RigidBody3DComponent c;
            c.velocity = transform(spec["velocity"].as<glm::vec3>());
            c.gravity = transform(spec["gravity"].as<bool>());
            c.frozen = transform(spec["frozen"].as<bool>());
            c.bounciness = transform(spec["bounciness"].as<float>());
            c.frictionCoefficient = transform(spec["frictionCoefficient"].as<float>());
            c.rollingResistance = transform(spec["rollingResistance"].as<float>());
            e.add<RigidBody3DComponent>(c);
        }
        if (auto spec = entity["BoxCollider3DComponent"]) {
            BoxCollider3DComponent c;
            c.position = transform(spec["position"].as<glm::vec3>());
            c.orientation = transform(spec["orientation"].as<glm::quat>());
            c.mass = transform(spec["mass"].as<float>());
            c.halfExtents = transform(spec["halfExtents"].as<glm::vec3>());
            c.applyScale = transform(spec["applyScale"].as<bool>());
            e.add<BoxCollider3DComponent>(c);
        }
        if (auto spec = entity["SphereCollider3DComponent"]) {
            SphereCollider3DComponent c;
            c.position = transform(spec["position"].as<glm::vec3>());
            c.orientation = transform(spec["orientation"].as<glm::quat>());
            c.mass = transform(spec["mass"].as<float>());
            c.radius = transform(spec["radius"].as<float>());
            e.add<SphereCollider3DComponent>(c);
        }
        if (auto spec = entity["CapsuleCollider3DComponent"]) {
            CapsuleCollider3DComponent c;
            c.position = transform(spec["position"].as<glm::vec3>());
            c.orientation = transform(spec["orientation"].as<glm::quat>());
            c.mass = transform(spec["mass"].as<float>());
            c.radius = transform(spec["radius"].as<float>());
            c.height = transform(spec["height"].as<float>());
            e.add<CapsuleCollider3DComponent>(c);
        }
        if (auto spec = entity["ScriptComponent"]) {
            ScriptComponent c;
            c.script = transform(spec["script"].as<std::string>());
            c.active = transform(spec["active"].as<bool>());
            e.add<ScriptComponent>(c);
        }
        if (auto spec = entity["Camera3DComponent"]) {
            Camera3DComponent c;
            c.fov = transform(spec["fov"].as<float>());
            c.pitch = transform(spec["pitch"].as<float>());
            e.add<Camera3DComponent>(c);
        }
        if (auto spec = entity["SelectComponent"]) {
            SelectComponent c;
            e.add<SelectComponent>(c);
        }
        if (auto spec = entity["PathComponent"]) {
            PathComponent c;
            c.speed = transform(spec["speed"].as<float>());
            e.add<PathComponent>(c);
        }
        if (auto spec = entity["GridComponent"]) {
            GridComponent c;
            c.x = transform(spec["x"].as<int>());
            c.z = transform(spec["z"].as<int>());
            e.add<GridComponent>(c);
        }
        if (auto spec = entity["LightComponent"]) {
            LightComponent c;
            c.colour = transform(spec["colour"].as<glm::vec3>());
            c.brightness = transform(spec["brightness"].as<float>());
            e.add<LightComponent>(c);
        }
        if (auto spec = entity["SunComponent"]) {
            SunComponent c;
            c.colour = transform(spec["colour"].as<glm::vec3>());
            c.brightness = transform(spec["brightness"].as<float>());
            c.direction = transform(spec["direction"].as<glm::vec3>());
            c.shadows = transform(spec["shadows"].as<bool>());
            e.add<SunComponent>(c);
        }
        if (auto spec = entity["AmbienceComponent"]) {
            AmbienceComponent c;
            c.colour = transform(spec["colour"].as<glm::vec3>());
            c.brightness = transform(spec["brightness"].as<float>());
            e.add<AmbienceComponent>(c);
        }
        if (auto spec = entity["ParticleComponent"]) {
            ParticleComponent c;
            c.interval = transform(spec["interval"].as<float>());
            c.velocity = transform(spec["velocity"].as<glm::vec3>());
            c.velocityNoise = transform(spec["velocityNoise"].as<float>());
            c.acceleration = transform(spec["acceleration"].as<glm::vec3>());
            c.scale = transform(spec["scale"].as<glm::vec3>());
            c.life = transform(spec["life"].as<float>());
            e.add<ParticleComponent>(c);
        }
        if (auto spec = entity["MeshAnimationComponent"]) {
            MeshAnimationComponent c;
            c.name = transform(spec["name"].as<std::string>());
            c.time = transform(spec["time"].as<float>());
            c.speed = transform(spec["speed"].as<float>());
            e.add<MeshAnimationComponent>(c);
        }
    }
}

ecs::Entity Copy(ecs::Registry* reg, ecs::Entity entity)
{
    ecs::Entity e = reg->create();
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
    for (auto entity : source->all()) {
        Copy(target, entity);
    }
}

}
}
