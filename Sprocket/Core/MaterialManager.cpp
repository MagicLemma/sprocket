#include "MaterialManager.h"
#include "Yaml.h"

#include <yaml-cpp/yaml.h>
#include <fstream>
#include <filesystem>

namespace Sprocket {
    
MaterialManager::MaterialManager(TextureManager* textureManager)
    : d_textureManager(textureManager)
    , d_default(std::make_shared<Material>())
{}

std::shared_ptr<Material> MaterialManager::GetMaterial(const std::string& file)
{
    if (file == "") { return d_default; }
    std::string filepath = std::filesystem::absolute(file).string();

    if (auto it = d_loadedMaterials.find(filepath); it != d_loadedMaterials.end()) {
        return it->second;
    }

    auto material = Material::FromFile(filepath);
    d_loadedMaterials.emplace(filepath, material);
    return material;
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