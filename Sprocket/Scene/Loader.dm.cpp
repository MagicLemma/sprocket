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
DATAMATIC_BEGIN SAVABLE=true
        if (reg->has<{{Comp::name}}>(entity)) {
            const auto& c = reg->get<{{Comp::name}}>(entity);
            out << YAML::Key << "{{Comp::name}}" << YAML::BeginMap;
            out << YAML::Key << "{{Attr::name}}" << YAML::Value << c.{{Attr::name}};
            out << YAML::EndMap;
        }
DATAMATIC_END
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
DATAMATIC_BEGIN SAVABLE=true
        if (auto spec = yaml_entity["{{Comp::name}}"]) {
            {{Comp::name}} c;
            c.{{Attr::name}} = {{Attr::parse_value_from_spec}};
            reg->add<{{Comp::name}}>(entity, c);
        }
DATAMATIC_END
    }
}

spkt::entity copy_entity(spkt::registry* reg, spkt::entity entity)
{
    spkt::entity new_entity = reg->create();
DATAMATIC_BEGIN SAVABLE=true
    if (reg->has<{{Comp::name}}>(entity)) {
        reg->add<{{Comp::name}}>(new_entity, reg->get<{{Comp::name}}>(entity));
    }
DATAMATIC_END
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
DATAMATIC_BEGIN SAVABLE=true
        if (source->has<{{Comp::name}}>(old_entity)) {
            const {{Comp::name}}& source_comp = source->get<{{Comp::name}}>(old_entity);
            {{Comp::name}} target_comp;
            target_comp.{{Attr::name}} = {{Attr::parse_value_copy}};
            target->add<{{Comp::name}}>(new_entity, target_comp);
        }
DATAMATIC_END
    }
}

}
