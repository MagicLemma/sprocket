#include "Loader.h"
#include "Log.h"
#include "Components.h"
#include "Maths.h"
#include "Yaml.h"
#include "Scene.h"
#include "Updater.h"

#include <yaml-cpp/yaml.h>
#include <fstream>
#include <memory>

namespace Sprocket {
namespace Loader {

void Save(const std::string& file, std::shared_ptr<Scene> scene)
{
    YAML::Emitter out;
    out << YAML::BeginMap;
    out << YAML::Key << "Version" << YAML::Value << 2;

    out << YAML::Key << "Entities" << YAML::BeginSeq;
    for (auto entity : scene->Reg()->Fast()) {
        if (entity.Has<TemporaryComponent>()) { return; }
        out << YAML::BeginMap;
#ifdef DATAMATIC_BLOCK SAVABLE=true
        if (entity.Has<{{Comp.Name}}>()) {
            const auto& c = entity.Get<{{Comp.Name}}>();
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

void Load(const std::string& file, std::shared_ptr<Scene> scene)
{
    // Must be a clean scene
    u32 count = 0;
    for (ECS::Entity e : scene->Reg()->Fast()) {
        if (!e.Has<TemporaryComponent>()) ++count;
    }
    assert(count == 0);

    std::ifstream stream(file);
    std::stringstream sstream;
    sstream << stream.rdbuf();

    YAML::Node data = YAML::Load(sstream.str());
    UpdateScene(data);

    if (!data["Entities"]) {
        return; // TODO: Error checking
    }

    auto entities = data["Entities"];
    for (auto entity : entities) {
        ECS::Entity e = scene->Reg()->New();
#ifdef DATAMATIC_BLOCK SAVABLE=true
        if (auto spec = entity["{{Comp.Name}}"]) {
            {{Comp.Name}} c;
            c.{{Attr.Name}} = spec["{{Attr.Name}}"] ? spec["{{Attr.Name}}"].as<{{Attr.Type}}>() : {{Attr.Default}};
            e.Add<{{Comp.Name}}>(c);
        }
#endif
    }
}

ECS::Entity Copy(std::shared_ptr<Scene> scene, ECS::Entity entity)
{
    ECS::Entity e = scene->Reg()->New();
#ifdef DATAMATIC_BLOCK
    if (entity.Has<{{Comp.Name}}>()) {
        e.Add<{{Comp.Name}}>(entity.Get<{{Comp.Name}}>());
    }
#endif
    return e;
}

void Copy(std::shared_ptr<Scene> source, std::shared_ptr<Scene> target)
{
    for (auto entity : source->Reg()->Fast()) {
        Copy(target, entity);
    }
}

}
}
