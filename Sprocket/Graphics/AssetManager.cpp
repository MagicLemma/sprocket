#include "AssetManager.h"

#include <filesystem>

namespace spkt {

AssetManager::AssetManager()
    : d_defaultMaterial(std::make_unique<Material>())
{
}

const Material& AssetManager::get_material(std::string_view file)
{
    if (file == "") { return *d_defaultMaterial; }
    std::string filepath = std::filesystem::absolute(file).string();

    if (auto it = d_materials.find(filepath); it != d_materials.end()) {
        return *it->second;
    }

    auto material = Material::FromFile(filepath);
    Material* ret = material.get();
    d_materials.emplace(filepath, std::move(material));
    return *ret;
}

bool AssetManager::is_loading_anything() const
{
    return is_loading_textures()
        || is_loading_static_meshes()
        || is_loading_animated_meshes();
}

}