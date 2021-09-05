#include "AssetManager.h"

#include <filesystem>

namespace spkt {

AssetManager::AssetManager()
    : d_default_static_mesh(std::make_unique<static_mesh>())
    , d_default_animated_mesh(std::make_unique<animated_mesh>())
    , d_defaultTexture(std::make_unique<texture>())
    , d_defaultMaterial(std::make_unique<Material>())
{
}

static_mesh* AssetManager::get_static_mesh(std::string_view file)
{
    if (file == "") { return d_default_static_mesh.get(); }
    std::string filepath = std::filesystem::absolute(file).string();

    if (auto it = d_static_meshes.find(filepath); it != d_static_meshes.end()) {
        return it->second.get();
    }

    if (auto it = d_loading_static_meshes.find(filepath); it != d_loading_static_meshes.end()) {
        if (it->second.wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
            auto mesh = std::make_unique<static_mesh>(*(it->second.get()));
            auto* ret = mesh.get();
            d_loading_static_meshes.erase(it);
            d_static_meshes.emplace(filepath, std::move(mesh));
            return ret;
        }
    } else {
        d_loading_static_meshes[filepath] = std::async(std::launch::async, [filepath]() {
            return std::make_unique<static_mesh_data>(static_mesh_data::load(filepath));
        });
    }

    return d_default_static_mesh.get();
}

animated_mesh* AssetManager::get_animated_mesh(std::string_view file)
{
    if (file == "") { return d_default_animated_mesh.get(); }
    std::string filepath = std::filesystem::absolute(file).string();

    if (auto it = d_animated_meshes.find(filepath); it != d_animated_meshes.end()) {
        return it->second.get();
    }

    if (auto it = d_loading_animated_meshes.find(filepath); it != d_loading_animated_meshes.end()) {
        if (it->second.wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
            auto mesh = std::make_unique<animated_mesh>(*(it->second.get()));
            auto* ret = mesh.get();
            d_loading_animated_meshes.erase(it);
            d_animated_meshes.emplace(filepath, std::move(mesh));
            return ret;
        }
    } else {
        d_loading_animated_meshes[filepath] = std::async(std::launch::async, [filepath]() {
            return std::make_unique<animated_mesh_data>(animated_mesh_data::load(filepath));
        });
    }

    return d_default_animated_mesh.get();
}

texture* AssetManager::GetTexture(std::string_view file)
{
    if (file == "") { return d_defaultTexture.get(); }
    std::string filepath = std::filesystem::absolute(file).string();

    if (auto it = d_textures.find(filepath); it != d_textures.end()) {
        return it->second.get();
    }

    if (auto it = d_loadingTextures.find(filepath); it != d_loadingTextures.end()) {
        if (it->second.wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
            auto tex = texture::from_data(*(it->second.get()));
            spkt::texture* ret = tex.get();
            d_loadingTextures.erase(it);
            d_textures.emplace(filepath, std::move(tex));
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

bool AssetManager::is_loading_static_meshes() const
{
    return d_loading_static_meshes.size() > 0;
}

bool AssetManager::is_loading_animated_meshes() const
{
    return d_loading_animated_meshes.size() > 0;
}

bool AssetManager::IsLoadingTextures() const
{
    return d_loadingTextures.size() > 0;
}

bool AssetManager::IsLoadingAnything() const
{
    return IsLoadingTextures()
        || is_loading_static_meshes()
        || is_loading_animated_meshes();
}

}