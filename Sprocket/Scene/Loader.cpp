#include "Loader.h"
#include "Log.h"
#include "ecs.h"
#include "Maths.h"
#include "Yaml.h"
#include "Scene.h"

#include <yaml-cpp/yaml.h>
#include <fstream>
#include <memory>

namespace spkt {
namespace {

// When loading entities from disk, their IDs may already be in use, so we assigned them
// new IDs when they are loaded. Because some components may store entity handles, we have
// to also map those to the new values. This current soluton is not that scalable, because
// if we ever use another container templatised on spkt::entity, another branch has to be
// added here.

using remapper_t = std::unordered_map<spkt::entity, spkt::entity>;

template <typename T>
T transform_entity(const remapper_t& remapper, T param) {
    using U = std::decay_t<T>;
    if constexpr (std::is_same_v<U, spkt::entity>) {
        return remapper.at(param);
    } else if constexpr (std::is_same_v<U, std::unordered_map<glm::ivec2, spkt::entity>>) {
        for (auto& entry : param) {
            entry.second = remapper.at(entry.second);
        }
        return param;
    } else {
        static_assert(false);
    }
};

}

void save_registry_to_file(const std::string& file, spkt::registry* reg)
{
    YAML::Emitter out;
    out << YAML::BeginMap;
    out << YAML::Key << "Entities" << YAML::BeginSeq;
    for (auto entity : reg->all()) {

        // Don't save runtime entities
        if (reg->has<Runtime>(entity)) { continue; }

        out << YAML::BeginMap;
        out << YAML::Key << "ID#" << YAML::Value << entity;
        if (reg->has<NameComponent>(entity)) {
            const auto& c = reg->get<NameComponent>(entity);
            out << YAML::Key << "NameComponent" << YAML::BeginMap;
            out << YAML::Key << "name" << YAML::Value << c.name;
            out << YAML::EndMap;
        }
        if (reg->has<Transform2DComponent>(entity)) {
            const auto& c = reg->get<Transform2DComponent>(entity);
            out << YAML::Key << "Transform2DComponent" << YAML::BeginMap;
            out << YAML::Key << "position" << YAML::Value << c.position;
            out << YAML::Key << "rotation" << YAML::Value << c.rotation;
            out << YAML::Key << "scale" << YAML::Value << c.scale;
            out << YAML::EndMap;
        }
        if (reg->has<Transform3DComponent>(entity)) {
            const auto& c = reg->get<Transform3DComponent>(entity);
            out << YAML::Key << "Transform3DComponent" << YAML::BeginMap;
            out << YAML::Key << "position" << YAML::Value << c.position;
            out << YAML::Key << "orientation" << YAML::Value << c.orientation;
            out << YAML::Key << "scale" << YAML::Value << c.scale;
            out << YAML::EndMap;
        }
        if (reg->has<ModelComponent>(entity)) {
            const auto& c = reg->get<ModelComponent>(entity);
            out << YAML::Key << "ModelComponent" << YAML::BeginMap;
            out << YAML::Key << "mesh" << YAML::Value << c.mesh;
            out << YAML::Key << "material" << YAML::Value << c.material;
            out << YAML::EndMap;
        }
        if (reg->has<RigidBody3DComponent>(entity)) {
            const auto& c = reg->get<RigidBody3DComponent>(entity);
            out << YAML::Key << "RigidBody3DComponent" << YAML::BeginMap;
            out << YAML::Key << "velocity" << YAML::Value << c.velocity;
            out << YAML::Key << "gravity" << YAML::Value << c.gravity;
            out << YAML::Key << "frozen" << YAML::Value << c.frozen;
            out << YAML::Key << "bounciness" << YAML::Value << c.bounciness;
            out << YAML::Key << "frictionCoefficient" << YAML::Value << c.frictionCoefficient;
            out << YAML::Key << "rollingResistance" << YAML::Value << c.rollingResistance;
            out << YAML::EndMap;
        }
        if (reg->has<BoxCollider3DComponent>(entity)) {
            const auto& c = reg->get<BoxCollider3DComponent>(entity);
            out << YAML::Key << "BoxCollider3DComponent" << YAML::BeginMap;
            out << YAML::Key << "position" << YAML::Value << c.position;
            out << YAML::Key << "orientation" << YAML::Value << c.orientation;
            out << YAML::Key << "mass" << YAML::Value << c.mass;
            out << YAML::Key << "halfExtents" << YAML::Value << c.halfExtents;
            out << YAML::Key << "applyScale" << YAML::Value << c.applyScale;
            out << YAML::EndMap;
        }
        if (reg->has<SphereCollider3DComponent>(entity)) {
            const auto& c = reg->get<SphereCollider3DComponent>(entity);
            out << YAML::Key << "SphereCollider3DComponent" << YAML::BeginMap;
            out << YAML::Key << "position" << YAML::Value << c.position;
            out << YAML::Key << "orientation" << YAML::Value << c.orientation;
            out << YAML::Key << "mass" << YAML::Value << c.mass;
            out << YAML::Key << "radius" << YAML::Value << c.radius;
            out << YAML::EndMap;
        }
        if (reg->has<CapsuleCollider3DComponent>(entity)) {
            const auto& c = reg->get<CapsuleCollider3DComponent>(entity);
            out << YAML::Key << "CapsuleCollider3DComponent" << YAML::BeginMap;
            out << YAML::Key << "position" << YAML::Value << c.position;
            out << YAML::Key << "orientation" << YAML::Value << c.orientation;
            out << YAML::Key << "mass" << YAML::Value << c.mass;
            out << YAML::Key << "radius" << YAML::Value << c.radius;
            out << YAML::Key << "height" << YAML::Value << c.height;
            out << YAML::EndMap;
        }
        if (reg->has<ScriptComponent>(entity)) {
            const auto& c = reg->get<ScriptComponent>(entity);
            out << YAML::Key << "ScriptComponent" << YAML::BeginMap;
            out << YAML::Key << "script" << YAML::Value << c.script;
            out << YAML::Key << "active" << YAML::Value << c.active;
            out << YAML::EndMap;
        }
        if (reg->has<Camera3DComponent>(entity)) {
            const auto& c = reg->get<Camera3DComponent>(entity);
            out << YAML::Key << "Camera3DComponent" << YAML::BeginMap;
            out << YAML::Key << "fov" << YAML::Value << c.fov;
            out << YAML::Key << "pitch" << YAML::Value << c.pitch;
            out << YAML::EndMap;
        }
        if (reg->has<PathComponent>(entity)) {
            const auto& c = reg->get<PathComponent>(entity);
            out << YAML::Key << "PathComponent" << YAML::BeginMap;
            out << YAML::Key << "speed" << YAML::Value << c.speed;
            out << YAML::EndMap;
        }
        if (reg->has<LightComponent>(entity)) {
            const auto& c = reg->get<LightComponent>(entity);
            out << YAML::Key << "LightComponent" << YAML::BeginMap;
            out << YAML::Key << "colour" << YAML::Value << c.colour;
            out << YAML::Key << "brightness" << YAML::Value << c.brightness;
            out << YAML::EndMap;
        }
        if (reg->has<SunComponent>(entity)) {
            const auto& c = reg->get<SunComponent>(entity);
            out << YAML::Key << "SunComponent" << YAML::BeginMap;
            out << YAML::Key << "colour" << YAML::Value << c.colour;
            out << YAML::Key << "brightness" << YAML::Value << c.brightness;
            out << YAML::Key << "direction" << YAML::Value << c.direction;
            out << YAML::Key << "shadows" << YAML::Value << c.shadows;
            out << YAML::EndMap;
        }
        if (reg->has<AmbienceComponent>(entity)) {
            const auto& c = reg->get<AmbienceComponent>(entity);
            out << YAML::Key << "AmbienceComponent" << YAML::BeginMap;
            out << YAML::Key << "colour" << YAML::Value << c.colour;
            out << YAML::Key << "brightness" << YAML::Value << c.brightness;
            out << YAML::EndMap;
        }
        if (reg->has<ParticleComponent>(entity)) {
            const auto& c = reg->get<ParticleComponent>(entity);
            out << YAML::Key << "ParticleComponent" << YAML::BeginMap;
            out << YAML::Key << "interval" << YAML::Value << c.interval;
            out << YAML::Key << "velocity" << YAML::Value << c.velocity;
            out << YAML::Key << "velocityNoise" << YAML::Value << c.velocityNoise;
            out << YAML::Key << "acceleration" << YAML::Value << c.acceleration;
            out << YAML::Key << "scale" << YAML::Value << c.scale;
            out << YAML::Key << "life" << YAML::Value << c.life;
            out << YAML::EndMap;
        }
        if (reg->has<MeshAnimationComponent>(entity)) {
            const auto& c = reg->get<MeshAnimationComponent>(entity);
            out << YAML::Key << "MeshAnimationComponent" << YAML::BeginMap;
            out << YAML::Key << "name" << YAML::Value << c.name;
            out << YAML::Key << "time" << YAML::Value << c.time;
            out << YAML::Key << "speed" << YAML::Value << c.speed;
            out << YAML::EndMap;
        }
        if (reg->has<TileMapSingleton>(entity)) {
            const auto& c = reg->get<TileMapSingleton>(entity);
            out << YAML::Key << "TileMapSingleton" << YAML::BeginMap;
            out << YAML::Key << "tiles" << YAML::Value << c.tiles;
            out << YAML::EndMap;
        }
        out << YAML::EndMap;
    }
    out << YAML::EndSeq;
    out << YAML::EndMap;

    std::ofstream(file) << out.c_str();
}

void load_registry_from_file(const std::string& file, spkt::registry* reg)
{
    std::ifstream stream(file);
    std::stringstream sstream;
    sstream << stream.rdbuf();

    YAML::Node data = YAML::Load(sstream.str());

    if (!data["Entities"]) {
        return; // TODO: Error checking
    }

    auto entities = data["Entities"];
    std::unordered_map<spkt::entity, spkt::entity> id_remapper;

    for (auto yaml_entity : entities) {
        spkt::entity old_id = yaml_entity["ID#"].as<spkt::entity>();
        spkt::entity new_id = reg->create();
        id_remapper[old_id] = new_id;
    }

    for (auto yaml_entity : entities) {
        spkt::entity entity = id_remapper[yaml_entity["ID#"].as<spkt::entity>()];
        if (auto spec = yaml_entity["NameComponent"]) {
            NameComponent c;
            c.name = spec["name"].as<std::string>();
            reg->add<NameComponent>(entity, c);
        }
        if (auto spec = yaml_entity["Transform2DComponent"]) {
            Transform2DComponent c;
            c.position = spec["position"].as<glm::vec2>();
            c.rotation = spec["rotation"].as<float>();
            c.scale = spec["scale"].as<glm::vec2>();
            reg->add<Transform2DComponent>(entity, c);
        }
        if (auto spec = yaml_entity["Transform3DComponent"]) {
            Transform3DComponent c;
            c.position = spec["position"].as<glm::vec3>();
            c.orientation = spec["orientation"].as<glm::quat>();
            c.scale = spec["scale"].as<glm::vec3>();
            reg->add<Transform3DComponent>(entity, c);
        }
        if (auto spec = yaml_entity["ModelComponent"]) {
            ModelComponent c;
            c.mesh = spec["mesh"].as<std::string>();
            c.material = spec["material"].as<std::string>();
            reg->add<ModelComponent>(entity, c);
        }
        if (auto spec = yaml_entity["RigidBody3DComponent"]) {
            RigidBody3DComponent c;
            c.velocity = spec["velocity"].as<glm::vec3>();
            c.gravity = spec["gravity"].as<bool>();
            c.frozen = spec["frozen"].as<bool>();
            c.bounciness = spec["bounciness"].as<float>();
            c.frictionCoefficient = spec["frictionCoefficient"].as<float>();
            c.rollingResistance = spec["rollingResistance"].as<float>();
            reg->add<RigidBody3DComponent>(entity, c);
        }
        if (auto spec = yaml_entity["BoxCollider3DComponent"]) {
            BoxCollider3DComponent c;
            c.position = spec["position"].as<glm::vec3>();
            c.orientation = spec["orientation"].as<glm::quat>();
            c.mass = spec["mass"].as<float>();
            c.halfExtents = spec["halfExtents"].as<glm::vec3>();
            c.applyScale = spec["applyScale"].as<bool>();
            reg->add<BoxCollider3DComponent>(entity, c);
        }
        if (auto spec = yaml_entity["SphereCollider3DComponent"]) {
            SphereCollider3DComponent c;
            c.position = spec["position"].as<glm::vec3>();
            c.orientation = spec["orientation"].as<glm::quat>();
            c.mass = spec["mass"].as<float>();
            c.radius = spec["radius"].as<float>();
            reg->add<SphereCollider3DComponent>(entity, c);
        }
        if (auto spec = yaml_entity["CapsuleCollider3DComponent"]) {
            CapsuleCollider3DComponent c;
            c.position = spec["position"].as<glm::vec3>();
            c.orientation = spec["orientation"].as<glm::quat>();
            c.mass = spec["mass"].as<float>();
            c.radius = spec["radius"].as<float>();
            c.height = spec["height"].as<float>();
            reg->add<CapsuleCollider3DComponent>(entity, c);
        }
        if (auto spec = yaml_entity["ScriptComponent"]) {
            ScriptComponent c;
            c.script = spec["script"].as<std::string>();
            c.active = spec["active"].as<bool>();
            reg->add<ScriptComponent>(entity, c);
        }
        if (auto spec = yaml_entity["Camera3DComponent"]) {
            Camera3DComponent c;
            c.fov = spec["fov"].as<float>();
            c.pitch = spec["pitch"].as<float>();
            reg->add<Camera3DComponent>(entity, c);
        }
        if (auto spec = yaml_entity["PathComponent"]) {
            PathComponent c;
            c.speed = spec["speed"].as<float>();
            reg->add<PathComponent>(entity, c);
        }
        if (auto spec = yaml_entity["LightComponent"]) {
            LightComponent c;
            c.colour = spec["colour"].as<glm::vec3>();
            c.brightness = spec["brightness"].as<float>();
            reg->add<LightComponent>(entity, c);
        }
        if (auto spec = yaml_entity["SunComponent"]) {
            SunComponent c;
            c.colour = spec["colour"].as<glm::vec3>();
            c.brightness = spec["brightness"].as<float>();
            c.direction = spec["direction"].as<glm::vec3>();
            c.shadows = spec["shadows"].as<bool>();
            reg->add<SunComponent>(entity, c);
        }
        if (auto spec = yaml_entity["AmbienceComponent"]) {
            AmbienceComponent c;
            c.colour = spec["colour"].as<glm::vec3>();
            c.brightness = spec["brightness"].as<float>();
            reg->add<AmbienceComponent>(entity, c);
        }
        if (auto spec = yaml_entity["ParticleComponent"]) {
            ParticleComponent c;
            c.interval = spec["interval"].as<float>();
            c.velocity = spec["velocity"].as<glm::vec3>();
            c.velocityNoise = spec["velocityNoise"].as<float>();
            c.acceleration = spec["acceleration"].as<glm::vec3>();
            c.scale = spec["scale"].as<glm::vec3>();
            c.life = spec["life"].as<float>();
            reg->add<ParticleComponent>(entity, c);
        }
        if (auto spec = yaml_entity["MeshAnimationComponent"]) {
            MeshAnimationComponent c;
            c.name = spec["name"].as<std::string>();
            c.time = spec["time"].as<float>();
            c.speed = spec["speed"].as<float>();
            reg->add<MeshAnimationComponent>(entity, c);
        }
        if (auto spec = yaml_entity["TileMapSingleton"]) {
            TileMapSingleton c;
            c.tiles = transform_entity(id_remapper, spec["tiles"].as<std::unordered_map<glm::ivec2, spkt::entity>>());
            reg->add<TileMapSingleton>(entity, c);
        }
    }
}

spkt::entity copy_entity(spkt::registry* reg, spkt::entity entity)
{
    spkt::entity new_entity = reg->create();
    if (reg->has<NameComponent>(entity)) {
        reg->add<NameComponent>(new_entity, reg->get<NameComponent>(entity));
    }
    if (reg->has<Transform2DComponent>(entity)) {
        reg->add<Transform2DComponent>(new_entity, reg->get<Transform2DComponent>(entity));
    }
    if (reg->has<Transform3DComponent>(entity)) {
        reg->add<Transform3DComponent>(new_entity, reg->get<Transform3DComponent>(entity));
    }
    if (reg->has<ModelComponent>(entity)) {
        reg->add<ModelComponent>(new_entity, reg->get<ModelComponent>(entity));
    }
    if (reg->has<RigidBody3DComponent>(entity)) {
        reg->add<RigidBody3DComponent>(new_entity, reg->get<RigidBody3DComponent>(entity));
    }
    if (reg->has<BoxCollider3DComponent>(entity)) {
        reg->add<BoxCollider3DComponent>(new_entity, reg->get<BoxCollider3DComponent>(entity));
    }
    if (reg->has<SphereCollider3DComponent>(entity)) {
        reg->add<SphereCollider3DComponent>(new_entity, reg->get<SphereCollider3DComponent>(entity));
    }
    if (reg->has<CapsuleCollider3DComponent>(entity)) {
        reg->add<CapsuleCollider3DComponent>(new_entity, reg->get<CapsuleCollider3DComponent>(entity));
    }
    if (reg->has<ScriptComponent>(entity)) {
        reg->add<ScriptComponent>(new_entity, reg->get<ScriptComponent>(entity));
    }
    if (reg->has<Camera3DComponent>(entity)) {
        reg->add<Camera3DComponent>(new_entity, reg->get<Camera3DComponent>(entity));
    }
    if (reg->has<PathComponent>(entity)) {
        reg->add<PathComponent>(new_entity, reg->get<PathComponent>(entity));
    }
    if (reg->has<LightComponent>(entity)) {
        reg->add<LightComponent>(new_entity, reg->get<LightComponent>(entity));
    }
    if (reg->has<SunComponent>(entity)) {
        reg->add<SunComponent>(new_entity, reg->get<SunComponent>(entity));
    }
    if (reg->has<AmbienceComponent>(entity)) {
        reg->add<AmbienceComponent>(new_entity, reg->get<AmbienceComponent>(entity));
    }
    if (reg->has<ParticleComponent>(entity)) {
        reg->add<ParticleComponent>(new_entity, reg->get<ParticleComponent>(entity));
    }
    if (reg->has<MeshAnimationComponent>(entity)) {
        reg->add<MeshAnimationComponent>(new_entity, reg->get<MeshAnimationComponent>(entity));
    }
    if (reg->has<TileMapSingleton>(entity)) {
        reg->add<TileMapSingleton>(new_entity, reg->get<TileMapSingleton>(entity));
    }
    return new_entity;
}

void copy_registry(spkt::registry* source, spkt::registry* target)
{
    // First, set up new handles in the target scene and create a mapping between
    // new and old IDs.
    std::unordered_map<spkt::entity, spkt::entity> id_remapper;
    for (auto id : source->all()) {
        id_remapper[id] = target->create();;
    }

    for (auto old_entity : source->all()) {
        spkt::entity new_entity = id_remapper.at(old_entity);
        if (source->has<NameComponent>(old_entity)) {
            const NameComponent& source_comp = source->get<NameComponent>(old_entity);
            NameComponent target_comp;
            target_comp.name = source_comp.name;
            target->add<NameComponent>(new_entity, target_comp);
        }
        if (source->has<Transform2DComponent>(old_entity)) {
            const Transform2DComponent& source_comp = source->get<Transform2DComponent>(old_entity);
            Transform2DComponent target_comp;
            target_comp.position = source_comp.position;
            target_comp.rotation = source_comp.rotation;
            target_comp.scale = source_comp.scale;
            target->add<Transform2DComponent>(new_entity, target_comp);
        }
        if (source->has<Transform3DComponent>(old_entity)) {
            const Transform3DComponent& source_comp = source->get<Transform3DComponent>(old_entity);
            Transform3DComponent target_comp;
            target_comp.position = source_comp.position;
            target_comp.orientation = source_comp.orientation;
            target_comp.scale = source_comp.scale;
            target->add<Transform3DComponent>(new_entity, target_comp);
        }
        if (source->has<ModelComponent>(old_entity)) {
            const ModelComponent& source_comp = source->get<ModelComponent>(old_entity);
            ModelComponent target_comp;
            target_comp.mesh = source_comp.mesh;
            target_comp.material = source_comp.material;
            target->add<ModelComponent>(new_entity, target_comp);
        }
        if (source->has<RigidBody3DComponent>(old_entity)) {
            const RigidBody3DComponent& source_comp = source->get<RigidBody3DComponent>(old_entity);
            RigidBody3DComponent target_comp;
            target_comp.velocity = source_comp.velocity;
            target_comp.gravity = source_comp.gravity;
            target_comp.frozen = source_comp.frozen;
            target_comp.bounciness = source_comp.bounciness;
            target_comp.frictionCoefficient = source_comp.frictionCoefficient;
            target_comp.rollingResistance = source_comp.rollingResistance;
            target->add<RigidBody3DComponent>(new_entity, target_comp);
        }
        if (source->has<BoxCollider3DComponent>(old_entity)) {
            const BoxCollider3DComponent& source_comp = source->get<BoxCollider3DComponent>(old_entity);
            BoxCollider3DComponent target_comp;
            target_comp.position = source_comp.position;
            target_comp.orientation = source_comp.orientation;
            target_comp.mass = source_comp.mass;
            target_comp.halfExtents = source_comp.halfExtents;
            target_comp.applyScale = source_comp.applyScale;
            target->add<BoxCollider3DComponent>(new_entity, target_comp);
        }
        if (source->has<SphereCollider3DComponent>(old_entity)) {
            const SphereCollider3DComponent& source_comp = source->get<SphereCollider3DComponent>(old_entity);
            SphereCollider3DComponent target_comp;
            target_comp.position = source_comp.position;
            target_comp.orientation = source_comp.orientation;
            target_comp.mass = source_comp.mass;
            target_comp.radius = source_comp.radius;
            target->add<SphereCollider3DComponent>(new_entity, target_comp);
        }
        if (source->has<CapsuleCollider3DComponent>(old_entity)) {
            const CapsuleCollider3DComponent& source_comp = source->get<CapsuleCollider3DComponent>(old_entity);
            CapsuleCollider3DComponent target_comp;
            target_comp.position = source_comp.position;
            target_comp.orientation = source_comp.orientation;
            target_comp.mass = source_comp.mass;
            target_comp.radius = source_comp.radius;
            target_comp.height = source_comp.height;
            target->add<CapsuleCollider3DComponent>(new_entity, target_comp);
        }
        if (source->has<ScriptComponent>(old_entity)) {
            const ScriptComponent& source_comp = source->get<ScriptComponent>(old_entity);
            ScriptComponent target_comp;
            target_comp.script = source_comp.script;
            target_comp.active = source_comp.active;
            target->add<ScriptComponent>(new_entity, target_comp);
        }
        if (source->has<Camera3DComponent>(old_entity)) {
            const Camera3DComponent& source_comp = source->get<Camera3DComponent>(old_entity);
            Camera3DComponent target_comp;
            target_comp.fov = source_comp.fov;
            target_comp.pitch = source_comp.pitch;
            target->add<Camera3DComponent>(new_entity, target_comp);
        }
        if (source->has<PathComponent>(old_entity)) {
            const PathComponent& source_comp = source->get<PathComponent>(old_entity);
            PathComponent target_comp;
            target_comp.speed = source_comp.speed;
            target->add<PathComponent>(new_entity, target_comp);
        }
        if (source->has<LightComponent>(old_entity)) {
            const LightComponent& source_comp = source->get<LightComponent>(old_entity);
            LightComponent target_comp;
            target_comp.colour = source_comp.colour;
            target_comp.brightness = source_comp.brightness;
            target->add<LightComponent>(new_entity, target_comp);
        }
        if (source->has<SunComponent>(old_entity)) {
            const SunComponent& source_comp = source->get<SunComponent>(old_entity);
            SunComponent target_comp;
            target_comp.colour = source_comp.colour;
            target_comp.brightness = source_comp.brightness;
            target_comp.direction = source_comp.direction;
            target_comp.shadows = source_comp.shadows;
            target->add<SunComponent>(new_entity, target_comp);
        }
        if (source->has<AmbienceComponent>(old_entity)) {
            const AmbienceComponent& source_comp = source->get<AmbienceComponent>(old_entity);
            AmbienceComponent target_comp;
            target_comp.colour = source_comp.colour;
            target_comp.brightness = source_comp.brightness;
            target->add<AmbienceComponent>(new_entity, target_comp);
        }
        if (source->has<ParticleComponent>(old_entity)) {
            const ParticleComponent& source_comp = source->get<ParticleComponent>(old_entity);
            ParticleComponent target_comp;
            target_comp.interval = source_comp.interval;
            target_comp.velocity = source_comp.velocity;
            target_comp.velocityNoise = source_comp.velocityNoise;
            target_comp.acceleration = source_comp.acceleration;
            target_comp.scale = source_comp.scale;
            target_comp.life = source_comp.life;
            target->add<ParticleComponent>(new_entity, target_comp);
        }
        if (source->has<MeshAnimationComponent>(old_entity)) {
            const MeshAnimationComponent& source_comp = source->get<MeshAnimationComponent>(old_entity);
            MeshAnimationComponent target_comp;
            target_comp.name = source_comp.name;
            target_comp.time = source_comp.time;
            target_comp.speed = source_comp.speed;
            target->add<MeshAnimationComponent>(new_entity, target_comp);
        }
        if (source->has<TileMapSingleton>(old_entity)) {
            const TileMapSingleton& source_comp = source->get<TileMapSingleton>(old_entity);
            TileMapSingleton target_comp;
            target_comp.tiles = transform_entity(id_remapper, source_comp.tiles);
            target->add<TileMapSingleton>(new_entity, target_comp);
        }
    }
}

}
