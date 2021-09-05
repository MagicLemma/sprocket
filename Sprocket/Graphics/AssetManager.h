#pragma once
#include <Sprocket/Graphics/Material.h>
#include <Sprocket/Graphics/mesh.h>
#include <Sprocket/Graphics/Texture.h>

#include <future>
#include <memory>
#include <ranges>
#include <string_view>
#include <string>
#include <unordered_map>

namespace spkt {

using texture_ptr = std::unique_ptr<texture>;
using material_ptr = std::unique_ptr<Material>;

class AssetManager
{
    // Background Loaders
    std::unordered_map<std::string, std::future<std::unique_ptr<static_mesh_data>>> d_loading_static_meshes;
    std::unordered_map<std::string, std::future<std::unique_ptr<animated_mesh_data>>> d_loading_animated_meshes;
    std::unordered_map<std::string, std::future<std::unique_ptr<texture_data>>> d_loadingTextures;

    // Primitives
    std::unordered_map<std::string, const spkt::static_mesh_ptr> d_static_meshes;
    std::unordered_map<std::string, const spkt::animated_mesh_ptr> d_animated_meshes;
    std::unordered_map<std::string, const spkt::texture_ptr> d_textures;
    
    // Composites
    std::unordered_map<std::string, const spkt::material_ptr> d_materials;

    // Defaults
    spkt::static_mesh_ptr d_default_static_mesh;
    spkt::animated_mesh_ptr d_default_animated_mesh;
    spkt::texture_ptr  d_defaultTexture;
    spkt::material_ptr d_defaultMaterial;

public:
    AssetManager();

    const static_mesh&   get_static_mesh   (std::string_view file);
    const animated_mesh& get_animated_mesh (std::string_view file);
    const texture&       get_texture       (std::string_view file);
    const Material&      get_material      (std::string_view file);

    auto static_meshes()   { return std::views::all(d_static_meshes); }
    auto animated_meshes() { return std::views::all(d_animated_meshes); }
    auto textures()        { return std::views::all(d_textures); }
    auto materials()       { return std::views::all(d_materials); }

    bool IsLoadingMeshes() const;
    bool is_loading_static_meshes() const;
    bool is_loading_animated_meshes() const;
    bool IsLoadingTextures() const;
    bool IsLoadingAnything() const;
};

}