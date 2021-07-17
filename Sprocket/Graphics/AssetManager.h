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

using mesh_ptr = std::unique_ptr<Mesh>;
using texture_ptr = std::unique_ptr<Texture>;
using material_ptr = std::unique_ptr<Material>;

class AssetManager
{
    // Background Loaders
    std::unordered_map<std::string, std::future<std::unique_ptr<MeshData>>>    d_loadingMeshes;
    std::unordered_map<std::string, std::future<std::unique_ptr<TextureData>>> d_loadingTextures;

    // Primitives
    std::unordered_map<std::string, const spkt::mesh_ptr>    d_meshes;
    std::unordered_map<std::string, const spkt::texture_ptr> d_textures;
    
    // Composites
    std::unordered_map<std::string, const spkt::material_ptr> d_materials;

    // Defaults
    spkt::mesh_ptr     d_defaultMesh;
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

    Mesh* GetMesh(std::string_view file);
    Texture* GetTexture(std::string_view file);
    Material* GetMaterial(std::string_view file);

    auto Meshes()    { return Iterator(&d_meshes); }
    auto Textures()  { return Iterator(&d_textures); }
    auto Materials() { return Iterator(&d_materials); }

    bool IsLoadingMeshes() const;
    bool IsLoadingTextures() const;
    bool IsLoadingAnything() const;
};

}