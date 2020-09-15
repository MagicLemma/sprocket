#pragma once
#include "TextureManager.h"
#include "Material.h"

#include <unordered_map>
#include <memory>
#include <string>

namespace Sprocket {

class MaterialManager
{
public:
    using Map = std::unordered_map<
        std::string,
        std::shared_ptr<Material>
    >;

private:
    TextureManager* d_textureManager;
    Map             d_loadedMaterials;

public:
    MaterialManager(TextureManager* textureManager);

    std::shared_ptr<Material> GetMaterial(const std::string& file);
    void SaveMaterial(const std::string& file, std::shared_ptr<Material> material);
};

}