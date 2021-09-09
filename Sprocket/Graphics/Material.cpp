#include "Material.h"

#include <Sprocket/Utility/Yaml.h>

#include <yaml-cpp/yaml.h>

#include <fstream>
#include <filesystem>

namespace spkt {

Material Material::load(std::string_view file)
{
    std::string filepath = std::filesystem::absolute(file).string();
    Material material;
    material.file = filepath;

    std::ifstream stream(filepath);
    std::stringstream sstream;
    sstream << stream.rdbuf();

    YAML::Node data = YAML::Load(sstream.str());

    if (auto name = data["Name"]) {
        material.name = name.as<std::string>();
    }
    else {
        material.name = "Bad material";
    }

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

std::unique_ptr<Material> Material::FromFile(const std::string& file)
{
    return std::make_unique<Material>(Material::load(file));
}

void Material::Save() const
{
    assert(std::filesystem::exists(file));
    YAML::Emitter out;
    auto K = YAML::Key, V = YAML::Value;

    out << YAML::BeginMap;

    out << K << "Name" << V << name;

    out << K << "AlbedoMap" << V << albedoMap
        << K << "NormalMap" << V << normalMap
        << K << "MetallicMap" << V << metallicMap
        << K << "RoughnessMap" << V << roughnessMap;

    out << K << "UseAlbedoMap" << V << useAlbedoMap
        << K << "UseNormalMap" << V << useNormalMap
        << K << "UseMetallicMap" << V << useMetallicMap
        << K << "UseRoughnessMap" << V << useRoughnessMap;

    out << K << "Albedo" << V << albedo
        << K << "Metallic" << V << metallic
        << K << "Roughness" << V << roughness;

    out << YAML::EndMap;
    std::ofstream fout(file);
    fout << out.c_str();
}

}