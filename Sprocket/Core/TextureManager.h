#pragma once
#include "Texture.h"

#include <string>
#include <vector>
#include <map>

namespace Sprocket {

class TextureManager
{
public:
    using Map = std::map<std::string, Texture>;

private:
    Map d_loadedTextures;

public:
    Texture GetTexture(const std::string& path);
        // Returns the texture in the specified path. If it has not
        // been loaded it will be loaded and stored internally.
};

}