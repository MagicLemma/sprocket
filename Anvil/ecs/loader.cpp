#include "loader.h"
#include "meta.h"
#include "ecs.h"
#include "lua_ecs.h"
#include "scene.h"

#include <Sprocket/Core/log.h>
#include <Sprocket/Utility/Yaml.h>

#include <yaml-cpp/yaml.h>

#include <fstream>
#include <ranges>
#include <memory>

namespace anvil {
namespace {

// When loading entities from disk, their IDs may already be in use, so we assigned them
// new IDs when they are loaded. Because some components may store entity handles, we have
// to also map those to the new values. This current soluton is not that scalable, because
// if we ever use another container templatised on entity, another branch has to be
// added here.

using remapper_t = std::unordered_map<entity, entity>;

template <typename T>
T transform_entity(const remapper_t& remapper, T param) {
    using U = std::decay_t<T>;
    if constexpr (std::is_same_v<U, entity>) {
        return remapper.at(param);
    } else if constexpr (std::is_same_v<U, std::unordered_map<glm::ivec2, entity>>) {
        for (auto& entry : param) {
            entry.second = remapper.at(entry.second);
        }
        return param;
    } else {
        static_assert(false);
    }
};

}

void save_registry_to_file(
    const std::string& file,
    const registry& reg,
    const save_predicate& predicate)
{
    YAML::Emitter out;
    out << YAML::BeginMap;
    out << YAML::Key << "Entities" << YAML::BeginSeq;

    const auto pred = [&](entity e) { return predicate(reg, e); };
    for (auto entity : reg.all() | std::views::filter(pred)) {

        out << YAML::BeginMap;
        out << YAML::Key << "ID#" << YAML::Value << entity;
        for_each_component([&]<typename T>(reflcomp<T>&& refl) {
            if constexpr (refl.is_savable()) {
                if (reg.has<T>(entity)) {
                    const auto& c = reg.get<T>(entity);
                    out << YAML::Key << refl.name << YAML::BeginMap;
                    refl.for_each_attribute(c, [&](auto&& attr_refl) {
                        if constexpr (attr_refl.is_savable()) {
                            out << YAML::Key << std::string{attr_refl.name}
                                << YAML::Value << *attr_refl.value;
                        }
                    });
                    out << YAML::EndMap;
                }
            }
        });
        out << YAML::EndMap;
    }
    out << YAML::EndSeq;
    out << YAML::EndMap;

    std::ofstream(file) << out.c_str();
}

void load_registry_from_file(const std::string& file, registry& reg)
{
    std::ifstream stream(file);
    std::stringstream sstream;
    sstream << stream.rdbuf();

    YAML::Node data = YAML::Load(sstream.str());

    if (!data["Entities"]) {
        return; // TODO: Error checking
    }

    auto entities = data["Entities"];
    std::unordered_map<entity, entity> id_remapper;

    for (auto yaml_entity : entities) {
        entity old_id = yaml_entity["ID#"].as<entity>();
        entity new_id = reg.create();
        id_remapper[old_id] = new_id;
    }

    for (auto yaml_entity : entities) {
        entity e = id_remapper[yaml_entity["ID#"].as<entity>()];
        if (auto spec = yaml_entity["NameComponent"]) {
            NameComponent c;
            c.name = spec["name"].as<std::string>();
            reg.add<NameComponent>(e, c);
        }
        if (auto spec = yaml_entity["Transform3DComponent"]) {
            Transform3DComponent c;
            c.position = spec["position"].as<glm::vec3>();
            c.orientation = spec["orientation"].as<glm::quat>();
            c.scale = spec["scale"].as<glm::vec3>();
            reg.add<Transform3DComponent>(e, c);
        }
        if (auto spec = yaml_entity["StaticModelComponent"]) {
            StaticModelComponent c;
            c.mesh = spec["mesh"].as<std::string>();
            c.material = spec["material"].as<std::string>();
            reg.add<StaticModelComponent>(e, c);
        }
        if (auto spec = yaml_entity["AnimatedModelComponent"]) {
            AnimatedModelComponent c;
            c.mesh = spec["mesh"].as<std::string>();
            c.material = spec["material"].as<std::string>();
            c.animation_name = spec["animation_name"].as<std::string>();
            c.animation_time = spec["animation_time"].as<float>();
            c.animation_speed = spec["animation_speed"].as<float>();
            reg.add<AnimatedModelComponent>(e, c);
        }
        if (auto spec = yaml_entity["RigidBody3DComponent"]) {
            RigidBody3DComponent c;
            c.velocity = spec["velocity"].as<glm::vec3>();
            c.gravity = spec["gravity"].as<bool>();
            c.frozen = spec["frozen"].as<bool>();
            c.bounciness = spec["bounciness"].as<float>();
            c.frictionCoefficient = spec["frictionCoefficient"].as<float>();
            c.rollingResistance = spec["rollingResistance"].as<float>();
            reg.add<RigidBody3DComponent>(e, c);
        }
        if (auto spec = yaml_entity["BoxCollider3DComponent"]) {
            BoxCollider3DComponent c;
            c.position = spec["position"].as<glm::vec3>();
            c.orientation = spec["orientation"].as<glm::quat>();
            c.mass = spec["mass"].as<float>();
            c.halfExtents = spec["halfExtents"].as<glm::vec3>();
            c.applyScale = spec["applyScale"].as<bool>();
            reg.add<BoxCollider3DComponent>(e, c);
        }
        if (auto spec = yaml_entity["SphereCollider3DComponent"]) {
            SphereCollider3DComponent c;
            c.position = spec["position"].as<glm::vec3>();
            c.orientation = spec["orientation"].as<glm::quat>();
            c.mass = spec["mass"].as<float>();
            c.radius = spec["radius"].as<float>();
            reg.add<SphereCollider3DComponent>(e, c);
        }
        if (auto spec = yaml_entity["CapsuleCollider3DComponent"]) {
            CapsuleCollider3DComponent c;
            c.position = spec["position"].as<glm::vec3>();
            c.orientation = spec["orientation"].as<glm::quat>();
            c.mass = spec["mass"].as<float>();
            c.radius = spec["radius"].as<float>();
            c.height = spec["height"].as<float>();
            reg.add<CapsuleCollider3DComponent>(e, c);
        }
        if (auto spec = yaml_entity["ScriptComponent"]) {
            ScriptComponent c;
            c.script = spec["script"].as<std::string>();
            c.active = spec["active"].as<bool>();
            reg.add<ScriptComponent>(e, c);
        }
        if (auto spec = yaml_entity["Camera3DComponent"]) {
            Camera3DComponent c;
            c.fov = spec["fov"].as<float>();
            c.pitch = spec["pitch"].as<float>();
            reg.add<Camera3DComponent>(e, c);
        }
        if (auto spec = yaml_entity["LightComponent"]) {
            LightComponent c;
            c.colour = spec["colour"].as<glm::vec3>();
            c.brightness = spec["brightness"].as<float>();
            reg.add<LightComponent>(e, c);
        }
        if (auto spec = yaml_entity["SunComponent"]) {
            SunComponent c;
            c.colour = spec["colour"].as<glm::vec3>();
            c.brightness = spec["brightness"].as<float>();
            c.direction = spec["direction"].as<glm::vec3>();
            c.shadows = spec["shadows"].as<bool>();
            reg.add<SunComponent>(e, c);
        }
        if (auto spec = yaml_entity["AmbienceComponent"]) {
            AmbienceComponent c;
            c.colour = spec["colour"].as<glm::vec3>();
            c.brightness = spec["brightness"].as<float>();
            reg.add<AmbienceComponent>(e, c);
        }
        if (auto spec = yaml_entity["ParticleComponent"]) {
            ParticleComponent c;
            c.interval = spec["interval"].as<float>();
            c.velocity = spec["velocity"].as<glm::vec3>();
            c.velocityNoise = spec["velocityNoise"].as<float>();
            c.acceleration = spec["acceleration"].as<glm::vec3>();
            c.scale = spec["scale"].as<glm::vec3>();
            c.life = spec["life"].as<float>();
            reg.add<ParticleComponent>(e, c);
        }
    }
}

entity copy_entity(registry& reg, entity e)
{
    entity new_entity = reg.create();
    for_each_component([&]<typename T>(reflcomp<T>&& refl) {
        if (refl.is_savable && reg.has<T>(e)) {
            reg.add<T>(new_entity, reg.get<T>(e));
        }
    });
    return new_entity;
}

void copy_registry(const registry& source, registry& target)
{
    // First, set up new handles in the target scene and create a mapping between
    // new and old IDs.
    std::unordered_map<entity, entity> id_remapper;
    for (auto id : source.all()) {
        id_remapper[id] = target.create();;
    }

    for (auto old_entity : source.all()) {
        entity new_entity = id_remapper.at(old_entity);
        if (source.has<NameComponent>(old_entity)) {
            const NameComponent& source_comp = source.get<NameComponent>(old_entity);
            NameComponent target_comp;
            target_comp.name = source_comp.name;
            target.add<NameComponent>(new_entity, target_comp);
        }
        if (source.has<Transform3DComponent>(old_entity)) {
            const Transform3DComponent& source_comp = source.get<Transform3DComponent>(old_entity);
            Transform3DComponent target_comp;
            target_comp.position = source_comp.position;
            target_comp.orientation = source_comp.orientation;
            target_comp.scale = source_comp.scale;
            target.add<Transform3DComponent>(new_entity, target_comp);
        }
        if (source.has<StaticModelComponent>(old_entity)) {
            const StaticModelComponent& source_comp = source.get<StaticModelComponent>(old_entity);
            StaticModelComponent target_comp;
            target_comp.mesh = source_comp.mesh;
            target_comp.material = source_comp.material;
            target.add<StaticModelComponent>(new_entity, target_comp);
        }
        if (source.has<AnimatedModelComponent>(old_entity)) {
            const AnimatedModelComponent& source_comp = source.get<AnimatedModelComponent>(old_entity);
            AnimatedModelComponent target_comp;
            target_comp.mesh = source_comp.mesh;
            target_comp.material = source_comp.material;
            target_comp.animation_name = source_comp.animation_name;
            target_comp.animation_time = source_comp.animation_time;
            target_comp.animation_speed = source_comp.animation_speed;
            target.add<AnimatedModelComponent>(new_entity, target_comp);
        }
        if (source.has<RigidBody3DComponent>(old_entity)) {
            const RigidBody3DComponent& source_comp = source.get<RigidBody3DComponent>(old_entity);
            RigidBody3DComponent target_comp;
            target_comp.velocity = source_comp.velocity;
            target_comp.gravity = source_comp.gravity;
            target_comp.frozen = source_comp.frozen;
            target_comp.bounciness = source_comp.bounciness;
            target_comp.frictionCoefficient = source_comp.frictionCoefficient;
            target_comp.rollingResistance = source_comp.rollingResistance;
            target.add<RigidBody3DComponent>(new_entity, target_comp);
        }
        if (source.has<BoxCollider3DComponent>(old_entity)) {
            const BoxCollider3DComponent& source_comp = source.get<BoxCollider3DComponent>(old_entity);
            BoxCollider3DComponent target_comp;
            target_comp.position = source_comp.position;
            target_comp.orientation = source_comp.orientation;
            target_comp.mass = source_comp.mass;
            target_comp.halfExtents = source_comp.halfExtents;
            target_comp.applyScale = source_comp.applyScale;
            target.add<BoxCollider3DComponent>(new_entity, target_comp);
        }
        if (source.has<SphereCollider3DComponent>(old_entity)) {
            const SphereCollider3DComponent& source_comp = source.get<SphereCollider3DComponent>(old_entity);
            SphereCollider3DComponent target_comp;
            target_comp.position = source_comp.position;
            target_comp.orientation = source_comp.orientation;
            target_comp.mass = source_comp.mass;
            target_comp.radius = source_comp.radius;
            target.add<SphereCollider3DComponent>(new_entity, target_comp);
        }
        if (source.has<CapsuleCollider3DComponent>(old_entity)) {
            const CapsuleCollider3DComponent& source_comp = source.get<CapsuleCollider3DComponent>(old_entity);
            CapsuleCollider3DComponent target_comp;
            target_comp.position = source_comp.position;
            target_comp.orientation = source_comp.orientation;
            target_comp.mass = source_comp.mass;
            target_comp.radius = source_comp.radius;
            target_comp.height = source_comp.height;
            target.add<CapsuleCollider3DComponent>(new_entity, target_comp);
        }
        if (source.has<ScriptComponent>(old_entity)) {
            const ScriptComponent& source_comp = source.get<ScriptComponent>(old_entity);
            ScriptComponent target_comp;
            target_comp.script = source_comp.script;
            target_comp.active = source_comp.active;
            target.add<ScriptComponent>(new_entity, target_comp);
        }
        if (source.has<Camera3DComponent>(old_entity)) {
            const Camera3DComponent& source_comp = source.get<Camera3DComponent>(old_entity);
            Camera3DComponent target_comp;
            target_comp.fov = source_comp.fov;
            target_comp.pitch = source_comp.pitch;
            target.add<Camera3DComponent>(new_entity, target_comp);
        }
        if (source.has<LightComponent>(old_entity)) {
            const LightComponent& source_comp = source.get<LightComponent>(old_entity);
            LightComponent target_comp;
            target_comp.colour = source_comp.colour;
            target_comp.brightness = source_comp.brightness;
            target.add<LightComponent>(new_entity, target_comp);
        }
        if (source.has<SunComponent>(old_entity)) {
            const SunComponent& source_comp = source.get<SunComponent>(old_entity);
            SunComponent target_comp;
            target_comp.colour = source_comp.colour;
            target_comp.brightness = source_comp.brightness;
            target_comp.direction = source_comp.direction;
            target_comp.shadows = source_comp.shadows;
            target.add<SunComponent>(new_entity, target_comp);
        }
        if (source.has<AmbienceComponent>(old_entity)) {
            const AmbienceComponent& source_comp = source.get<AmbienceComponent>(old_entity);
            AmbienceComponent target_comp;
            target_comp.colour = source_comp.colour;
            target_comp.brightness = source_comp.brightness;
            target.add<AmbienceComponent>(new_entity, target_comp);
        }
        if (source.has<ParticleComponent>(old_entity)) {
            const ParticleComponent& source_comp = source.get<ParticleComponent>(old_entity);
            ParticleComponent target_comp;
            target_comp.interval = source_comp.interval;
            target_comp.velocity = source_comp.velocity;
            target_comp.velocityNoise = source_comp.velocityNoise;
            target_comp.acceleration = source_comp.acceleration;
            target_comp.scale = source_comp.scale;
            target_comp.life = source_comp.life;
            target.add<ParticleComponent>(new_entity, target_comp);
        }
    }
}

}
