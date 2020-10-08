#pragma once
#include "Texture.h"

#include <string>
#include <vector>
#include <memory>
#include <map>

namespace Sprocket {

class TextureManager
{
public:
    using Map = std::map<std::string, std::shared_ptr<Texture>>;

private:
    Map d_loadedTextures;
    std::shared_ptr<Texture> d_default;

public:
    TextureManager();

    std::shared_ptr<Texture> GetTexture(const std::string& path);
        // Returns the texture in the specified path. If it has not
        // been loaded it will be loaded and stored internally.
};

}