#pragma once
#include "Mesh.h"
#include "Texture.h"
#include "Material.h"

#include <unordered_map>
#include <memory>
#include <string>

namespace Sprocket {

template <typename T>
class AssetIterator
{
    T* d_map;
public:
    AssetIterator(T* map) : d_map(map) {}
    auto begin() { return d_map->begin(); }
    auto end() { return d_map->end(); }
    auto cbegin() { return d_map->cbegin(); }
    auto cend() { return d_map->cend(); }
};

class AssetManager
{
    // Primitives
    std::unordered_map<std::string, std::shared_ptr<Mesh>>     d_meshes;
    std::unordered_map<std::string, std::shared_ptr<Texture>>  d_textures;
    
    // Composites
    std::unordered_map<std::string, std::shared_ptr<Material>> d_materials;

    // Defaults
    std::shared_ptr<Mesh>     d_defaultMesh;
    std::shared_ptr<Texture>  d_defaultTexture;
    std::shared_ptr<Material> d_defaultMaterial;

public:
    AssetManager();

    std::shared_ptr<Mesh> GetMesh(const std::string& file);
    std::shared_ptr<Texture> GetTexture(const std::string& file);
    std::shared_ptr<Material> GetMaterial(const std::string& file);

    auto Meshes()    { return AssetIterator(&d_meshes); }
    auto Textures()  { return AssetIterator(&d_textures); }
    auto Materials() { return AssetIterator(&d_materials); }
};

}