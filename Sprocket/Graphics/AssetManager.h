#pragma once
#include "Mesh.h"
#include "Texture.h"
#include "Material.h"

#include <unordered_map>
#include <memory>
#include <string>
#include <string_view>
#include <future>

namespace Sprocket {

class AssetManager
{
    // Background Loaders
    std::unordered_map<std::string, std::future<std::unique_ptr<MeshData>>>    d_loadingMeshes;
    std::unordered_map<std::string, std::future<std::unique_ptr<TextureData>>> d_loadingTextures;

    // Primitives
    std::unordered_map<std::string, const std::unique_ptr<Mesh>>    d_meshes;
    std::unordered_map<std::string, const std::unique_ptr<Texture>> d_textures;
    
    // Composites
    std::unordered_map<std::string, const std::unique_ptr<Material>> d_materials;

    // Defaults
    std::unique_ptr<Mesh>     d_defaultMesh;
    std::unique_ptr<Texture>  d_defaultTexture;
    std::unique_ptr<Material> d_defaultMaterial;

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