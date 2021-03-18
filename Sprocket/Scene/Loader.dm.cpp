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

    auto entities = data["Entities"];
    std::unordered_map<ecs::Identifier, ecs::Identifier> id_remapper;

    // Performs any extra transformations to values that cannot be done during
    // yaml decoding, for example converting entity IDs to their new values.
    const auto transform = [&](auto&& param) {
        if constexpr (std::is_same_v<decltype(param), ecs::Identifier>) {
            return id_remapper[param];
        }
        return param;
    };
    
    for (auto entity : entities) {
        ecs::Identifier old_id = entity["ID#"].as<ecs::Identifier>();
        ecs::Identifier new_id = reg->create().id();
        id_remapper[old_id] = new_id;
    }

    for (auto entity : entities) {
        ecs::Identifier old_id = entity["ID#"].as<ecs::Identifier>();
        ecs::Entity e{reg, id_remapper[old_id]};
#ifdef DATAMATIC_BLOCK SAVABLE=true
        if (auto spec = entity["{{Comp.Name}}"]) {
            {{Comp.Name}} c;
            c.{{Attr.Name}} = transform(spec["{{Attr.Name}}"].as<{{Attr.Type}}>());
            e.add<{{Comp.Name}}>(c);
        }
#endif
    }
}

ecs::Entity Copy(ecs::Registry* reg, ecs::Entity entity)
{
    ecs::Entity e = reg->create();
#ifdef DATAMATIC_BLOCK
    if (entity.has<{{Comp.Name}}>()) {
        e.add<{{Comp.Name}}>(entity.get<{{Comp.Name}}>());
    }
#endif
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
