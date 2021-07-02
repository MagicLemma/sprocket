#include "Loader.h"
#include "Log.h"
#include "Components.h"
#include "Maths.h"
#include "Yaml.h"
#include "Scene.h"

#include <yaml-cpp/yaml.h>
#include <fstream>
#include <memory>

namespace spkt {
namespace Loader {
namespace {

// When loading entities from disk, their IDs may already be in use, so we assigned them
// new IDs when they are loaded. Because some components may store entity handles, we have
// to also map those to the new values. This current soluton is not that scalable, because
// if we ever use another container templatised on apx::entity, another branch has to be
// added here.

using remapper_t = std::unordered_map<apx::entity, apx::entity>;

template <typename T>
T transform_entity(const remapper_t& remapper, T param) {
    using U = std::decay_t<T>;
    if constexpr (std::is_same_v<U, apx::entity>) {
        return remapper.at(param);
    } else if constexpr (std::is_same_v<U, std::unordered_map<glm::ivec2, apx::entity>>) {
        for (auto& entry : param) {
            entry.second = remapper.at(entry.second);
        }
        return param;
    } else {
        static_assert(false);
    }
};

}

void Save(const std::string& file, spkt::registry* reg)
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

void Load(const std::string& file, spkt::registry* reg)
{
    std::ifstream stream(file);
    std::stringstream sstream;
    sstream << stream.rdbuf();

    YAML::Node data = YAML::Load(sstream.str());

    if (!data["Entities"]) {
        return; // TODO: Error checking
    }

    auto entities = data["Entities"];
    std::unordered_map<apx::entity, apx::entity> id_remapper;

    for (auto yaml_entity : entities) {
        apx::entity old_id = yaml_entity["ID#"].as<apx::entity>();
        apx::entity new_id = reg->create();
        id_remapper[old_id] = new_id;
    }

    for (auto yaml_entity : entities) {
        apx::entity entity = id_remapper[yaml_entity["ID#"].as<apx::entity>()];
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
            c.tiles = transform_entity(id_remapper, spec["tiles"].as<std::unordered_map<glm::ivec2, apx::entity>>());
            reg->add<TileMapSingleton>(entity, c);
        }
    }
}

apx::entity Copy(spkt::registry* reg, apx::entity entity)
{
    apx::entity e = reg->create();
    if (reg->has<Runtime>(entity)) {
        reg->add<Runtime>(e, reg->get<Runtime>(entity));
    }
    if (reg->has<Singleton>(entity)) {
        reg->add<Singleton>(e, reg->get<Singleton>(entity));
    }
    if (reg->has<Event>(entity)) {
        reg->add<Event>(e, reg->get<Event>(entity));
    }
    if (reg->has<NameComponent>(entity)) {
        reg->add<NameComponent>(e, reg->get<NameComponent>(entity));
    }
    if (reg->has<Transform2DComponent>(entity)) {
        reg->add<Transform2DComponent>(e, reg->get<Transform2DComponent>(entity));
    }
    if (reg->has<Transform3DComponent>(entity)) {
        reg->add<Transform3DComponent>(e, reg->get<Transform3DComponent>(entity));
    }
    if (reg->has<ModelComponent>(entity)) {
        reg->add<ModelComponent>(e, reg->get<ModelComponent>(entity));
    }
    if (reg->has<RigidBody3DComponent>(entity)) {
        reg->add<RigidBody3DComponent>(e, reg->get<RigidBody3DComponent>(entity));
    }
    if (reg->has<BoxCollider3DComponent>(entity)) {
        reg->add<BoxCollider3DComponent>(e, reg->get<BoxCollider3DComponent>(entity));
    }
    if (reg->has<SphereCollider3DComponent>(entity)) {
        reg->add<SphereCollider3DComponent>(e, reg->get<SphereCollider3DComponent>(entity));
    }
    if (reg->has<CapsuleCollider3DComponent>(entity)) {
        reg->add<CapsuleCollider3DComponent>(e, reg->get<CapsuleCollider3DComponent>(entity));
    }
    if (reg->has<ScriptComponent>(entity)) {
        reg->add<ScriptComponent>(e, reg->get<ScriptComponent>(entity));
    }
    if (reg->has<Camera3DComponent>(entity)) {
        reg->add<Camera3DComponent>(e, reg->get<Camera3DComponent>(entity));
    }
    if (reg->has<PathComponent>(entity)) {
        reg->add<PathComponent>(e, reg->get<PathComponent>(entity));
    }
    if (reg->has<LightComponent>(entity)) {
        reg->add<LightComponent>(e, reg->get<LightComponent>(entity));
    }
    if (reg->has<SunComponent>(entity)) {
        reg->add<SunComponent>(e, reg->get<SunComponent>(entity));
    }
    if (reg->has<AmbienceComponent>(entity)) {
        reg->add<AmbienceComponent>(e, reg->get<AmbienceComponent>(entity));
    }
    if (reg->has<ParticleComponent>(entity)) {
        reg->add<ParticleComponent>(e, reg->get<ParticleComponent>(entity));
    }
    if (reg->has<MeshAnimationComponent>(entity)) {
        reg->add<MeshAnimationComponent>(e, reg->get<MeshAnimationComponent>(entity));
    }
    if (reg->has<CollisionEvent>(entity)) {
        reg->add<CollisionEvent>(e, reg->get<CollisionEvent>(entity));
    }
    if (reg->has<PhysicsSingleton>(entity)) {
        reg->add<PhysicsSingleton>(e, reg->get<PhysicsSingleton>(entity));
    }
    if (reg->has<InputSingleton>(entity)) {
        reg->add<InputSingleton>(e, reg->get<InputSingleton>(entity));
    }
    if (reg->has<GameGridSingleton>(entity)) {
        reg->add<GameGridSingleton>(e, reg->get<GameGridSingleton>(entity));
    }
    if (reg->has<TileMapSingleton>(entity)) {
        reg->add<TileMapSingleton>(e, reg->get<TileMapSingleton>(entity));
    }
    if (reg->has<CameraSingleton>(entity)) {
        reg->add<CameraSingleton>(e, reg->get<CameraSingleton>(entity));
    }
    if (reg->has<ParticleSingleton>(entity)) {
        reg->add<ParticleSingleton>(e, reg->get<ParticleSingleton>(entity));
    }
    return e;
}

void Copy(spkt::registry* source, spkt::registry* target)
{
    // First, set up new handles in the target scene and create a mapping between
    // new and old IDs.
    std::unordered_map<apx::entity, apx::entity> id_remapper;
    for (auto id : source->all()) {
        apx::entity new_id = target->create();
        id_remapper[id] = new_id;
    }

    for (auto id : source->all()) {
        spkt::entity src{*source, id};
        spkt::entity dst{*target, id_remapper[id]};
        if (src.has<Runtime>()) {
            const Runtime& source_comp = src.get<Runtime>();
            Runtime target_comp;
            dst.add<Runtime>(target_comp);
        }
        if (src.has<Singleton>()) {
            const Singleton& source_comp = src.get<Singleton>();
            Singleton target_comp;
            dst.add<Singleton>(target_comp);
        }
        if (src.has<Event>()) {
            const Event& source_comp = src.get<Event>();
            Event target_comp;
            dst.add<Event>(target_comp);
        }
        if (src.has<NameComponent>()) {
            const NameComponent& source_comp = src.get<NameComponent>();
            NameComponent target_comp;
            target_comp.name = source_comp.name;
            dst.add<NameComponent>(target_comp);
        }
        if (src.has<Transform2DComponent>()) {
            const Transform2DComponent& source_comp = src.get<Transform2DComponent>();
            Transform2DComponent target_comp;
            target_comp.position = source_comp.position;
            target_comp.rotation = source_comp.rotation;
            target_comp.scale = source_comp.scale;
            dst.add<Transform2DComponent>(target_comp);
        }
        if (src.has<Transform3DComponent>()) {
            const Transform3DComponent& source_comp = src.get<Transform3DComponent>();
            Transform3DComponent target_comp;
            target_comp.position = source_comp.position;
            target_comp.orientation = source_comp.orientation;
            target_comp.scale = source_comp.scale;
            dst.add<Transform3DComponent>(target_comp);
        }
        if (src.has<ModelComponent>()) {
            const ModelComponent& source_comp = src.get<ModelComponent>();
            ModelComponent target_comp;
            target_comp.mesh = source_comp.mesh;
            target_comp.material = source_comp.material;
            dst.add<ModelComponent>(target_comp);
        }
        if (src.has<RigidBody3DComponent>()) {
            const RigidBody3DComponent& source_comp = src.get<RigidBody3DComponent>();
            RigidBody3DComponent target_comp;
            target_comp.velocity = source_comp.velocity;
            target_comp.gravity = source_comp.gravity;
            target_comp.frozen = source_comp.frozen;
            target_comp.bounciness = source_comp.bounciness;
            target_comp.frictionCoefficient = source_comp.frictionCoefficient;
            target_comp.rollingResistance = source_comp.rollingResistance;
            target_comp.force = source_comp.force;
            target_comp.onFloor = source_comp.onFloor;
            target_comp.runtime = source_comp.runtime;
            dst.add<RigidBody3DComponent>(target_comp);
        }
        if (src.has<BoxCollider3DComponent>()) {
            const BoxCollider3DComponent& source_comp = src.get<BoxCollider3DComponent>();
            BoxCollider3DComponent target_comp;
            target_comp.position = source_comp.position;
            target_comp.orientation = source_comp.orientation;
            target_comp.mass = source_comp.mass;
            target_comp.halfExtents = source_comp.halfExtents;
            target_comp.applyScale = source_comp.applyScale;
            target_comp.runtime = source_comp.runtime;
            dst.add<BoxCollider3DComponent>(target_comp);
        }
        if (src.has<SphereCollider3DComponent>()) {
            const SphereCollider3DComponent& source_comp = src.get<SphereCollider3DComponent>();
            SphereCollider3DComponent target_comp;
            target_comp.position = source_comp.position;
            target_comp.orientation = source_comp.orientation;
            target_comp.mass = source_comp.mass;
            target_comp.radius = source_comp.radius;
            target_comp.runtime = source_comp.runtime;
            dst.add<SphereCollider3DComponent>(target_comp);
        }
        if (src.has<CapsuleCollider3DComponent>()) {
            const CapsuleCollider3DComponent& source_comp = src.get<CapsuleCollider3DComponent>();
            CapsuleCollider3DComponent target_comp;
            target_comp.position = source_comp.position;
            target_comp.orientation = source_comp.orientation;
            target_comp.mass = source_comp.mass;
            target_comp.radius = source_comp.radius;
            target_comp.height = source_comp.height;
            target_comp.runtime = source_comp.runtime;
            dst.add<CapsuleCollider3DComponent>(target_comp);
        }
        if (src.has<ScriptComponent>()) {
            const ScriptComponent& source_comp = src.get<ScriptComponent>();
            ScriptComponent target_comp;
            target_comp.script = source_comp.script;
            target_comp.active = source_comp.active;
            target_comp.script_runtime = source_comp.script_runtime;
            dst.add<ScriptComponent>(target_comp);
        }
        if (src.has<Camera3DComponent>()) {
            const Camera3DComponent& source_comp = src.get<Camera3DComponent>();
            Camera3DComponent target_comp;
            target_comp.projection = source_comp.projection;
            target_comp.fov = source_comp.fov;
            target_comp.pitch = source_comp.pitch;
            dst.add<Camera3DComponent>(target_comp);
        }
        if (src.has<PathComponent>()) {
            const PathComponent& source_comp = src.get<PathComponent>();
            PathComponent target_comp;
            target_comp.markers = source_comp.markers;
            target_comp.speed = source_comp.speed;
            dst.add<PathComponent>(target_comp);
        }
        if (src.has<LightComponent>()) {
            const LightComponent& source_comp = src.get<LightComponent>();
            LightComponent target_comp;
            target_comp.colour = source_comp.colour;
            target_comp.brightness = source_comp.brightness;
            dst.add<LightComponent>(target_comp);
        }
        if (src.has<SunComponent>()) {
            const SunComponent& source_comp = src.get<SunComponent>();
            SunComponent target_comp;
            target_comp.colour = source_comp.colour;
            target_comp.brightness = source_comp.brightness;
            target_comp.direction = source_comp.direction;
            target_comp.shadows = source_comp.shadows;
            dst.add<SunComponent>(target_comp);
        }
        if (src.has<AmbienceComponent>()) {
            const AmbienceComponent& source_comp = src.get<AmbienceComponent>();
            AmbienceComponent target_comp;
            target_comp.colour = source_comp.colour;
            target_comp.brightness = source_comp.brightness;
            dst.add<AmbienceComponent>(target_comp);
        }
        if (src.has<ParticleComponent>()) {
            const ParticleComponent& source_comp = src.get<ParticleComponent>();
            ParticleComponent target_comp;
            target_comp.interval = source_comp.interval;
            target_comp.velocity = source_comp.velocity;
            target_comp.velocityNoise = source_comp.velocityNoise;
            target_comp.acceleration = source_comp.acceleration;
            target_comp.scale = source_comp.scale;
            target_comp.life = source_comp.life;
            target_comp.accumulator = source_comp.accumulator;
            dst.add<ParticleComponent>(target_comp);
        }
        if (src.has<MeshAnimationComponent>()) {
            const MeshAnimationComponent& source_comp = src.get<MeshAnimationComponent>();
            MeshAnimationComponent target_comp;
            target_comp.name = source_comp.name;
            target_comp.time = source_comp.time;
            target_comp.speed = source_comp.speed;
            dst.add<MeshAnimationComponent>(target_comp);
        }
        if (src.has<CollisionEvent>()) {
            const CollisionEvent& source_comp = src.get<CollisionEvent>();
            CollisionEvent target_comp;
            target_comp.entity_a = transform_entity(id_remapper, source_comp.entity_a);
            target_comp.entity_b = transform_entity(id_remapper, source_comp.entity_b);
            dst.add<CollisionEvent>(target_comp);
        }
        if (src.has<PhysicsSingleton>()) {
            const PhysicsSingleton& source_comp = src.get<PhysicsSingleton>();
            PhysicsSingleton target_comp;
            target_comp.physics_runtime = source_comp.physics_runtime;
            dst.add<PhysicsSingleton>(target_comp);
        }
        if (src.has<InputSingleton>()) {
            const InputSingleton& source_comp = src.get<InputSingleton>();
            InputSingleton target_comp;
            target_comp.keyboard = source_comp.keyboard;
            target_comp.mouse = source_comp.mouse;
            target_comp.mouse_click = source_comp.mouse_click;
            target_comp.mouse_unclick = source_comp.mouse_unclick;
            target_comp.mouse_pos = source_comp.mouse_pos;
            target_comp.mouse_offset = source_comp.mouse_offset;
            target_comp.mouse_scrolled = source_comp.mouse_scrolled;
            target_comp.window_width = source_comp.window_width;
            target_comp.window_height = source_comp.window_height;
            target_comp.window_resized = source_comp.window_resized;
            dst.add<InputSingleton>(target_comp);
        }
        if (src.has<GameGridSingleton>()) {
            const GameGridSingleton& source_comp = src.get<GameGridSingleton>();
            GameGridSingleton target_comp;
            target_comp.hovered_square_entity = transform_entity(id_remapper, source_comp.hovered_square_entity);
            target_comp.clicked_square_entity = transform_entity(id_remapper, source_comp.clicked_square_entity);
            target_comp.hovered_square = source_comp.hovered_square;
            target_comp.clicked_square = source_comp.clicked_square;
            dst.add<GameGridSingleton>(target_comp);
        }
        if (src.has<TileMapSingleton>()) {
            const TileMapSingleton& source_comp = src.get<TileMapSingleton>();
            TileMapSingleton target_comp;
            target_comp.tiles = transform_entity(id_remapper, source_comp.tiles);
            dst.add<TileMapSingleton>(target_comp);
        }
        if (src.has<CameraSingleton>()) {
            const CameraSingleton& source_comp = src.get<CameraSingleton>();
            CameraSingleton target_comp;
            target_comp.camera_entity = transform_entity(id_remapper, source_comp.camera_entity);
            dst.add<CameraSingleton>(target_comp);
        }
        if (src.has<ParticleSingleton>()) {
            const ParticleSingleton& source_comp = src.get<ParticleSingleton>();
            ParticleSingleton target_comp;
            target_comp.particle_manager = source_comp.particle_manager;
            dst.add<ParticleSingleton>(target_comp);
        }
    }
}

}
}
