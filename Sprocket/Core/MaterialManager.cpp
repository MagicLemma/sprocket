#include "MaterialManager.h"
#include "Yaml.h"

#include <yaml-cpp/yaml.h>
#include <fstream>

namespace Sprocket {
    
MaterialManager::MaterialManager(TextureManager* textureManager)
    : d_textureManager(textureManager)
{}

std::shared_ptr<Material> MaterialManager::GetMaterial(const std::string& file)
{
    auto it = d_loadedMaterials.find(file);
    if (it != d_loadedMaterials.end()) {
        return it->second;
    }

    auto material = std::make_shared<Material>();
    material->file = file;

    std::ifstream stream(file);
    std::stringstream sstream;
    sstream << stream.rdbuf();

    YAML::Node data = YAML::Load(sstream.str());

    if (auto albedoMap = data["AlbedoMap"]) {
        material->albedoMap = d_textureManager->GetTexture(albedoMap.as<std::string>());
    }
    if (auto normalMap = data["NormalMap"]) {
        material->normalMap = d_textureManager->GetTexture(normalMap.as<std::string>());
    }
    if (auto metallicMap = data["MetallicMap"]) {
        material->metallicMap = d_textureManager->GetTexture(metallicMap.as<std::string>());
    }
    if (auto roughnessMap = data["RoughnessMap"]) {
        material->roughnessMap = d_textureManager->GetTexture(roughnessMap.as<std::string>());
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

    d_loadedMaterials.emplace(file, material);
    return material;
}

void MaterialManager::SaveMaterial(const std::string& file, std::shared_ptr<Material> material)
{
    YAML::Emitter out;
    assert(material->albedoMap.IsFromFile());
    assert(material->normalMap.IsFromFile());
    assert(material->metallicMap.IsFromFile());
    assert(material->roughnessMap.IsFromFile());

    out << YAML::BeginMap;

    auto K = YAML::Key;
    auto V = YAML::Value;

    out << K << "AlbedoMap" << V << material->albedoMap.Filepath()
        << K << "NormalMap" << V << material->normalMap.Filepath()
        << K << "MetallicMap" << V << material->metallicMap.Filepath()
        << K << "RoughnessMap" << V << material->roughnessMap.Filepath();

    out << K << "UseAlbedoMap" << V << material->useAlbedoMap
        << K << "UseNormalMap" << V << material->useNormalMap
        << K << "UseMetallicMap" << V << material->useMetallicMap
        << K << "UseRoughnessMap" << V << material->useRoughnessMap;

    out << K << "Albedo" << V << material->albedo
        << K << "Metallic" << V << material->metallic
        << K << "Roughness" << V << material->roughness;

    out << YAML::EndMap;
    std::ofstream fout(file);
    fout << out.c_str();
}

}