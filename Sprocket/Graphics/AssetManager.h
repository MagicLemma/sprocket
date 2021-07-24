#pragma once
#include <Sprocket/Graphics/Material.h>
#include <Sprocket/Graphics/Mesh.h>
#include <Sprocket/Graphics/Texture.h>

#include <unordered_map>
#include <memory>
#include <string>
#include <string_view>
#include <future>

namespace spkt {

using texture_ptr = std::unique_ptr<Texture>;
using material_ptr = std::unique_ptr<Material>;

class AssetManager
{
    // Background Loaders
    std::unordered_map<std::string, std::future<std::unique_ptr<StaticMeshData>>> d_loading_static_meshes;
    std::unordered_map<std::string, std::future<std::unique_ptr<AnimatedMeshData>>> d_loading_animated_meshes;
    std::unordered_map<std::string, std::future<std::unique_ptr<TextureData>>> d_loadingTextures;

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
    template <typename T>
    class Iterator
    {
        T* d_map;
    public:
        Iterator(T* map) : d_map(map) {}
        auto begin() { return d_map->begin(); }
        auto end() { return d_map->end(); }
        auto cbegin() { return d_map->cbegin(); }
        auto cend() { return d_map->cend(); }
    };

public:
    AssetManager();

    static_mesh* get_static_mesh(std::string_view file);
    animated_mesh* get_animated_mesh(std::string_view file);
    Texture* GetTexture(std::string_view file);
    Material* GetMaterial(std::string_view file);

    auto static_meshes()   { return Iterator(&d_static_meshes); }
    auto animated_meshes() { return Iterator(&d_animated_meshes); }
    auto Textures()        { return Iterator(&d_textures); }
    auto Materials()       { return Iterator(&d_materials); }

    bool IsLoadingMeshes() const;
    bool is_loading_static_meshes() const;
    bool is_loading_animated_meshes() const;
    bool IsLoadingTextures() const;
    bool IsLoadingAnything() const;
};

}