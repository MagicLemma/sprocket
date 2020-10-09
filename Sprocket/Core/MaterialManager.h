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
    TextureManager*           d_textureManager;
    Map                       d_loadedMaterials;
    std::shared_ptr<Material> d_default;

public:
    MaterialManager(TextureManager* textureManager);

    std::shared_ptr<Material> GetMaterial(const std::string& file);

    // Lower camel case to match STL iterators
    Map::iterator begin();
    Map::iterator end();
    Map::const_iterator cbegin() const;
    Map::const_iterator cend() const;
};

}