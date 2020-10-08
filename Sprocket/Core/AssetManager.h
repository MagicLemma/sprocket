#pragma once
#include "Mesh.h"
#include "Texture.h"
#include "Material.h"

#include <unordered_map>
#include <memory>
#include <string>

namespace Sprocket {

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
};

}