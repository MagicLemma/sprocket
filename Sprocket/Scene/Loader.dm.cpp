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

    const auto& sun = scene->GetSun();
    out << YAML::Key << "Sun" << YAML::BeginMap;
    out << YAML::Key << "direction" << YAML::Value << sun.direction;
    out << YAML::Key << "colour" << YAML::Value << sun.colour;
    out << YAML::Key << "brightness" << YAML::Value << sun.brightness;
    out << YAML::EndMap;

    const auto& ambience = scene->GetAmbience();
    out << YAML::Key << "Ambience" << YAML::BeginMap;
    out << YAML::Key << "colour" << YAML::Value << ambience.colour;
    out << YAML::Key << "brightness" << YAML::Value << ambience.brightness;
    out << YAML::EndMap;

    out << YAML::Key << "Entities" << YAML::BeginSeq;
    scene->All([&](ECS::Entity& entity) {
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
    });
    out << YAML::EndSeq;
    out << YAML::EndMap;

    std::ofstream fout(file);
    fout << out.c_str();
}

void Load(const std::string& file, std::shared_ptr<Scene> scene)
{
    scene->Clear();

    std::ifstream stream(file);
    std::stringstream sstream;
    sstream << stream.rdbuf();

    YAML::Node data = YAML::Load(sstream.str());
    UpdateScene(data);

    if (auto sun = data["Sun"]) {
        scene->GetSun().direction = sun["direction"] ? sun["direction"].as<glm::vec3>() : glm::vec3{0.0, -1.0, 0.0};
        scene->GetSun().colour = sun["colour"] ? sun["colour"].as<glm::vec3>() : glm::vec3{1.0, 1.0, 1.0};
        scene->GetSun().brightness = sun["brightness"] ? sun["brightness"].as<float>() : 1.0f;
    }

    if (auto ambience = data["Ambience"]) {
        scene->GetAmbience().colour = ambience["colour"] ? ambience["colour"].as<glm::vec3>() : glm::vec3{1.0, 1.0, 1.0};
        scene->GetAmbience().brightness = ambience["brightness"] ? ambience["brightness"].as<float>() : 1.0f;
    }

    if (!data["Entities"]) {
        return; // TODO: Error checking
    }

    auto entities = data["Entities"];
    for (auto entity : entities) {
        ECS::Entity e = scene->GetRegistry()->New();
#ifdef DATAMATIC_BLOCK SAVABLE=true
        if (auto spec = entity["{{Comp.Name}}"]) {
            {{Comp.Name}} c;
            c.{{Attr.Name}} = spec["{{Attr.Name}}"] ? spec["{{Attr.Name}}"].as<{{Attr.Type}}>() : {{Attr.Default}};
            e.Add(c);
        }
#endif
    }
}

ECS::Entity Copy(std::shared_ptr<Scene> scene, ECS::Entity entity)
{
    ECS::Entity e = scene->GetRegistry()->New();
#ifdef DATAMATIC_BLOCK
    if (entity.Has<{{Comp.Name}}>()) {
        e.Add<{{Comp.Name}}>(entity.Get<{{Comp.Name}}>());
    }
#endif
    return e;
}

void Copy(std::shared_ptr<Scene> source, std::shared_ptr<Scene> target)
{
    target->Clear();
    target->GetSun() = source->GetSun();
    target->GetAmbience() = source->GetAmbience();
    source->All([&](ECS::Entity& entity) {
        Copy(target, entity);
    });
}

}
}
