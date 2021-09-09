#include "Material.h"

#include <Sprocket/Utility/Log.h>
#include <Sprocket/Utility/Yaml.h>

#include <yaml-cpp/yaml.h>

#include <fstream>
#include <filesystem>

namespace spkt {

Material Material::load(std::string_view file)
{
    std::string filepath = std::filesystem::absolute(file).string();
    Material material;

    std::ifstream stream(filepath);
    std::stringstream sstream;
    sstream << stream.rdbuf();

    YAML::Node data = YAML::Load(sstream.str());

    material.name = data["Name"] ? data["Name"].as<std::string>() : "Bad material";

    if (auto albedoMap = data["AlbedoMap"]) {
        material.albedoMap = albedoMap.as<std::string>();
    }
    if (auto normalMap = data["NormalMap"]) {
        material.normalMap = normalMap.as<std::string>();
    }
    if (auto metallicMap = data["MetallicMap"]) {
        material.metallicMap = metallicMap.as<std::string>();
    }
    if (auto roughnessMap = data["RoughnessMap"]) {
        material.roughnessMap = roughnessMap.as<std::string>();
    }

    if (auto useAlbedoMap = data["UseAlbedoMap"]) {
        material.useAlbedoMap = useAlbedoMap.as<bool>();
    }
    if (auto useNormalMap = data["UseNormalMap"]) {
        material.useNormalMap = useNormalMap.as<bool>();
    }
    if (auto useMetallicMap = data["UseMetallicMap"]) {
        material.useMetallicMap = useMetallicMap.as<bool>();
    }
    if (auto useRoughnessMap = data["UseRoughnessMap"]) {
        material.useRoughnessMap = useRoughnessMap.as<bool>();
    }

    if (auto albedo = data["Albedo"]) {
        material.albedo = albedo.as<glm::vec3>();
    }
    if (auto metallic = data["Metallic"]) {
        material.metallic = metallic.as<float>();
    }
    if (auto roughness = data["Roughness"]) {
        material.roughness = roughness.as<float>();
    }

    return material;
}

void save_material(const std::string& file, const Material& material)
{
    if (!std::filesystem::exists(file)) {
        log::error("Could not save material: file {} does not exist", file);
        return;
    }

    YAML::Emitter out;
    out << YAML::BeginMap
        << YAML::Key << "Name"            << YAML::Value << material.name
        << YAML::Key << "AlbedoMap"       << YAML::Value << material.albedoMap
        << YAML::Key << "NormalMap"       << YAML::Value << material.normalMap
        << YAML::Key << "MetallicMap"     << YAML::Value << material.metallicMap
        << YAML::Key << "RoughnessMap"    << YAML::Value << material.roughnessMap
        << YAML::Key << "UseAlbedoMap"    << YAML::Value << material.useAlbedoMap
        << YAML::Key << "UseNormalMap"    << YAML::Value << material.useNormalMap
        << YAML::Key << "UseMetallicMap"  << YAML::Value << material.useMetallicMap
        << YAML::Key << "UseRoughnessMap" << YAML::Value << material.useRoughnessMap
        << YAML::Key << "Albedo"          << YAML::Value << material.albedo
        << YAML::Key << "Metallic"        << YAML::Value << material.metallic
        << YAML::Key << "Roughness"       << YAML::Value << material.roughness
        << YAML::EndMap;
    
    std::ofstream fout(file);
    fout << out.c_str();
}

}