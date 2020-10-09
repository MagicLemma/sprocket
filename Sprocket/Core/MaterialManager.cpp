#include "MaterialManager.h"
#include "Yaml.h"

#include <yaml-cpp/yaml.h>
#include <fstream>
#include <filesystem>

namespace Sprocket {
    
MaterialManager::MaterialManager(TextureManager* textureManager)
    : d_textureManager(textureManager)
{}

std::shared_ptr<Material> MaterialManager::GetMaterial(const std::string& file)
{
    std::string filepath = std::filesystem::absolute(file).string();
    if (filepath == "") {
        static auto defaultMaterial = std::make_shared<Material>();
        return defaultMaterial;
    }

    auto it = d_loadedMaterials.find(filepath);
    if (it != d_loadedMaterials.end()) {
        return it->second;
    }

    auto material = std::make_shared<Material>();
    material->file = filepath;

    std::ifstream stream(filepath);
    std::stringstream sstream;
    sstream << stream.rdbuf();

    YAML::Node data = YAML::Load(sstream.str());

    if (auto name = data["Name"]) {
        material->name = name.as<std::string>();
    }
    else {
        material->name = "Bad material";
    }

    if (auto albedoMap = data["AlbedoMap"]) {
        material->albedoMap = albedoMap.as<std::string>();
    }
    if (auto normalMap = data["NormalMap"]) {
        material->normalMap = normalMap.as<std::string>();
    }
    if (auto metallicMap = data["MetallicMap"]) {
        material->metallicMap = metallicMap.as<std::string>();
    }
    if (auto roughnessMap = data["RoughnessMap"]) {
        material->roughnessMap = roughnessMap.as<std::string>();
    }

    if (auto useAlbedoMap = data["UseAlbedoMap"]) {
        material->useAlbedoMap = useAlbedoMap.as<bool>();
    }
    if (auto useNormalMap = data["UseNormalMap"]) {
        material->useNormalMap = useNormalMap.as<bool>();
    }
    if (auto useMetallicMap = data["UseMetallicMap"]) {
        material->useMetallicMap = useMetallicMap.as<bool>();
    }
    if (auto useRoughnessMap = data["UseRoughnessMap"]) {
        material->useRoughnessMap = useRoughnessMap.as<bool>();
    }

    if (auto albedo = data["Albedo"]) {
        material->albedo = albedo.as<Maths::vec3>();
    }
    if (auto metallic = data["Metallic"]) {
        material->metallic = metallic.as<float>();
    }
    if (auto roughness = data["Roughness"]) {
        material->roughness = roughness.as<float>();
    }

    d_loadedMaterials.emplace(filepath, material);
    return material;
}

void MaterialManager::SaveMaterial(std::shared_ptr<Material> material)
{
    YAML::Emitter out;
    auto K = YAML::Key, V = YAML::Value;

    out << YAML::BeginMap;

    out << K << "Name" << V << material->name;

    out << K << "AlbedoMap" << V << material->albedoMap
        << K << "NormalMap" << V << material->normalMap
        << K << "MetallicMap" << V << material->metallicMap
        << K << "RoughnessMap" << V << material->roughnessMap;

    out << K << "UseAlbedoMap" << V << material->useAlbedoMap
        << K << "UseNormalMap" << V << material->useNormalMap
        << K << "UseMetallicMap" << V << material->useMetallicMap
        << K << "UseRoughnessMap" << V << material->useRoughnessMap;

    out << K << "Albedo" << V << material->albedo
        << K << "Metallic" << V << material->metallic
        << K << "Roughness" << V << material->roughness;

    out << YAML::EndMap;
    std::ofstream fout(material->file);
    fout << out.c_str();
}

MaterialManager::Map::iterator MaterialManager::begin()
{
    return d_loadedMaterials.begin();
}

MaterialManager::Map::iterator MaterialManager::end()
{
    return d_loadedMaterials.end();
}

MaterialManager::Map::const_iterator MaterialManager::cbegin() const
{
    return d_loadedMaterials.cbegin();
}

MaterialManager::Map::const_iterator MaterialManager::cend() const
{
    return d_loadedMaterials.cend();
}

}