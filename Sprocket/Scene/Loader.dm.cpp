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

void Save(const std::string& file, spkt::registry* reg)
{
    YAML::Emitter out;
    out << YAML::BeginMap;
    out << YAML::Key << "Entities" << YAML::BeginSeq;
    for (auto id : reg->all()) {
        spkt::entity entity{*reg, id};
        if (entity.has<TemporaryComponent>()) { return; }
        out << YAML::BeginMap;
        out << YAML::Key << "ID#" << YAML::Value << id;
DATAMATIC_BEGIN SAVABLE=true
        if (entity.has<{{Comp::name}}>()) {
            const auto& c = entity.get<{{Comp::name}}>();
            out << YAML::Key << "{{Comp::name}}" << YAML::BeginMap;
            out << YAML::Key << "{{Attr::name}}" << YAML::Value << c.{{Attr::name}};
            out << YAML::EndMap;
        }
DATAMATIC_END
        out << YAML::EndMap;
    }
    out << YAML::EndSeq;
    out << YAML::EndMap;

    std::ofstream fout(file);
    fout << out.c_str();
}

void Load(const std::string& file, spkt::registry* reg)
{
    // Must be a clean scene
    u32 count = 0;
    for (auto id : reg->all()) {
        spkt::entity entity{*reg, id};
        if (!entity.has<TemporaryComponent>()) ++count;
    }
    assert(count == 0);

    std::ifstream stream(file);
    std::stringstream sstream;
    sstream << stream.rdbuf();

    YAML::Node data = YAML::Load(sstream.str());

    if (!data["Entities"]) {
        return; // TODO: Error checking
    }

    auto entities = data["Entities"];
    std::unordered_map<spkt::identifier, spkt::identifier> id_remapper;

    // Performs any extra transformations to values that cannot be done during
    // yaml decoding, for example converting entity IDs to their new values.
    const auto transform = [&](auto&& param) {
        if constexpr (std::is_same_v<decltype(param), spkt::identifier>) {
            return id_remapper[param];
        }
        return param;
    };
    
    for (auto entity : entities) {
        spkt::identifier old_id = entity["ID#"].as<spkt::identifier>();
        spkt::identifier new_id = reg->create();
        id_remapper[old_id] = new_id;
    }

    for (auto entity : entities) {
        spkt::identifier old_id = entity["ID#"].as<spkt::identifier>();
        spkt::entity e{*reg, id_remapper[old_id]};
DATAMATIC_BEGIN SAVABLE=true
        if (auto spec = entity["{{Comp::name}}"]) {
            {{Comp::name}} c;
            c.{{Attr::name}} = transform(spec["{{Attr::name}}"].as<{{Attr::type}}>());
            e.add<{{Comp::name}}>(c);
        }
DATAMATIC_END
    }

    // Add the singleton entity if the scene does not have one.
    auto singleton = reg->find<Singleton>();
    if (!reg->valid(singleton)) {
        log::info("Adding a singleton entity to the loaded scene");
        singleton = reg->create();
        reg->emplace<Singleton>(singleton);
        reg->emplace<NameComponent>(singleton, "::Singleton");
    }
}

spkt::entity Copy(spkt::registry* reg, spkt::entity entity)
{
    spkt::entity e = apx::create_from(*reg);
DATAMATIC_BEGIN
    if (entity.has<{{Comp::name}}>()) {
        e.add<{{Comp::name}}>(entity.get<{{Comp::name}}>());
    }
DATAMATIC_END
    return e;
}

void Copy(spkt::registry* source, spkt::registry* target)
{
    // First, set up new handles in the target scene and create a mapping between
    // new and old IDs.
    std::unordered_map<spkt::identifier, spkt::identifier> id_remapper;
    for (auto id : source->all()) {
        spkt::identifier new_id = target->create();
        id_remapper[id] = new_id;
    }

    const auto transform = [&](auto&& param) {
        if constexpr (std::is_same_v<decltype(param), spkt::identifier>) {
            return id_remapper[param];
        }
        return param;
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
