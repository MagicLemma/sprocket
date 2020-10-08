#include "AssetManager.h"

#include <filesystem>

namespace Sprocket {

AssetManager::AssetManager()
    : d_defaultMesh(std::make_shared<Mesh>())
    , d_defaultTexture(std::make_shared<Texture>())
    , d_defaultMaterial(std::make_shared<Material>())
{
}

std::shared_ptr<Mesh> AssetManager::GetMesh(const std::string& file)
{
    if (file == "") { return d_defaultMesh; }
    std::string filepath = std::filesystem::absolute(file).string();

    if (auto it = d_meshes.find(filepath); it != d_meshes.end()) {
        return it->second;
    }

    auto model = Mesh::FromFile(filepath);
    d_meshes.emplace(filepath, model);
    return model;
}

std::shared_ptr<Texture> AssetManager::GetTexture(const std::string& file)
{
    if (file == "") { return d_defaultTexture; }
    std::string filepath = std::filesystem::absolute(file).string();

    if (auto it = d_textures.find(filepath); it != d_textures.end()) {
        return it->second;
    }

    auto texture = std::make_shared<Texture>(filepath);
    d_textures.emplace(filepath, texture);
    return texture;
}

std::shared_ptr<Material> AssetManager::GetMaterial(const std::string& file)
{
#if 0
    if (file == "") { return d_defaultMaterial; }
    std::string filepath = std::filesystem::absolute(file).string();

    if (auto it = d_materials.find(filepath); it != d_materials.end()) {
        return it->second;
    }

    auto material = std::make_shared<Material>(filepath);
    d_materials.emplace(filepath, material);
    return material;
#endif
    return nullptr;
}

}