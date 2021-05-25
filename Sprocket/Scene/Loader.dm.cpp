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
#ifdef DATAMATIC_BLOCK SAVABLE=true
        if (entity.has<{{Comp.Name}}>()) {
            const auto& c = entity.get<{{Comp.Name}}>();
            out << YAML::Key << "{{Comp.Name}}" << YAML::BeginMap;
            out << YAML::Key << "{{Attr.Name}}" << YAML::Value << c.{{Attr.Name}};
            out << YAML::EndMap;
        }
#endif
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
#ifdef DATAMATIC_BLOCK SAVABLE=true
        if (auto spec = entity["{{Comp.Name}}"]) {
            {{Comp.Name}} c;
            c.{{Attr.Name}} = transform(spec["{{Attr.Name}}"].as<{{Attr.Type}}>());
            e.add<{{Comp.Name}}>(c);
        }
#endif
    }
}

spkt::entity Copy(spkt::registry* reg, spkt::entity entity)
{
    spkt::entity e = apx::create_from(*reg);
#ifdef DATAMATIC_BLOCK
    if (entity.has<{{Comp.Name}}>()) {
        e.add<{{Comp.Name}}>(entity.get<{{Comp.Name}}>());
    }
#endif
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
#ifdef DATAMATIC_BLOCK
        if (src.has<{{Comp.Name}}>()) {
            const {{Comp.Name}}& source_comp = src.get<{{Comp.Name}}>();
            {{Comp.Name}} target_comp;
            target_comp.{{Attr.Name}} = transform(source_comp.{{Attr.Name}});
            dst.add<{{Comp.Name}}>(target_comp);
        }
#endif
    }
}

}
}
