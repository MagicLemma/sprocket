#include "AssetManager.h"

#include <filesystem>

namespace spkt {

AssetManager::AssetManager()
    : d_defaultMesh(std::make_unique<Mesh>())
    , d_defaultTexture(std::make_unique<Texture>())
    , d_defaultMaterial(std::make_unique<Material>())
{
}

Mesh* AssetManager::GetMesh(std::string_view file)
{
    if (file == "") { return d_defaultMesh.get(); }
    std::string filepath = std::filesystem::absolute(file).string();

    if (auto it = d_meshes.find(filepath); it != d_meshes.end()) {
        return it->second.get();
    }

    if (auto it = d_loadingMeshes.find(filepath); it != d_loadingMeshes.end()) {
        if (it->second.wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
            auto mesh = Mesh::FromData(*(it->second.get()));
            Mesh* ret = mesh.get();
            d_loadingMeshes.erase(it);
            d_meshes.emplace(filepath, std::move(mesh));
            return ret;
        }
    } else {
        d_loadingMeshes[filepath] = std::async(std::launch::async, [filepath]() {
            return std::make_unique<MeshData>(filepath);
        });
    }

    return d_defaultMesh.get();
}

Texture* AssetManager::GetTexture(std::string_view file)
{
    if (file == "") { return d_defaultTexture.get(); }
    std::string filepath = std::filesystem::absolute(file).string();

    if (auto it = d_textures.find(filepath); it != d_textures.end()) {
        return it->second.get();
    }

    if (auto it = d_loadingTextures.find(filepath); it != d_loadingTextures.end()) {
        if (it->second.wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
            auto texture = Texture::FromData(*(it->second.get()));
            Texture* ret = texture.get();
            d_loadingTextures.erase(it);
            d_textures.emplace(filepath, std::move(texture));
            return ret;
        }
    } else {
        d_loadingTextures[filepath] = std::async(std::launch::async, [filepath]() {
            return std::make_unique<TextureData>(filepath);
        });
    }

    return d_defaultTexture.get();
}

Material* AssetManager::GetMaterial(std::string_view file)
{
    if (file == "") { return d_defaultMaterial.get(); }
    std::string filepath = std::filesystem::absolute(file).string();

    if (auto it = d_materials.find(filepath); it != d_materials.end()) {
        return it->second.get();
    }

    auto material = Material::FromFile(filepath);
    Material* ret = material.get();
    d_materials.emplace(filepath, std::move(material));
    return ret;
}

bool AssetManager::IsLoadingMeshes() const
{
    return d_loadingMeshes.size() > 0;
}

bool AssetManager::IsLoadingTextures() const
{
    return d_loadingTextures.size() > 0;
}

bool AssetManager::IsLoadingAnything() const
{
    return IsLoadingMeshes() || IsLoadingTextures();
}

}