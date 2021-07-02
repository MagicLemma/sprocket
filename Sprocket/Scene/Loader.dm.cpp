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

    // Performs any extra transformations to values that cannot be done during
    // yaml decoding, for example converting entity IDs to their new values.
    const auto transform = [&] <typename T> (T&& param) {
        if constexpr (std::is_same_v<T, apx::entity>) {
            return id_remapper[param];
        }
        return param;
    };
    
    for (auto yaml_entity : entities) {
        apx::entity old_id = yaml_entity["ID#"].as<apx::entity>();
        apx::entity new_id = reg->create();
        id_remapper[old_id] = new_id;
    }

    for (auto yaml_entity : entities) {
        apx::entity entity = id_remapper[yaml_entity["ID#"].as<apx::entity>()];
DATAMATIC_BEGIN SAVABLE=true
        if (auto spec = yaml_entity["{{Comp::name}}"]) {
            {{Comp::name}} c;
            c.{{Attr::name}} = {{Attr::parse_value}};
            reg->add<{{Comp::name}}>(entity, c);
        }
DATAMATIC_END
    }
}

apx::entity Copy(spkt::registry* reg, apx::entity entity)
{
    apx::entity e = reg->create();
DATAMATIC_BEGIN
    if (reg->has<{{Comp::name}}>(entity)) {
        reg->add<{{Comp::name}}>(e, reg->get<{{Comp::name}}>(entity));
    }
DATAMATIC_END
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

    const auto transform = [&] <typename T> (T&& param) {
        if constexpr (std::is_same_v<decltype(param), apx::entity>) {
            return id_remapper[param];
        }
        return std::forward<T>(param);
    };

    for (auto id : source->all()) {
        spkt::entity src{*source, id};
        spkt::entity dst{*target, id_remapper[id]};
DATAMATIC_BEGIN
        if (src.has<{{Comp::name}}>()) {
            const {{Comp::name}}& source_comp = src.get<{{Comp::name}}>();
            {{Comp::name}} target_comp;
            target_comp.{{Attr::name}} = transform(source_comp.{{Attr::name}});
            dst.add<{{Comp::name}}>(target_comp);
        }
DATAMATIC_END
    }
}

}
}
