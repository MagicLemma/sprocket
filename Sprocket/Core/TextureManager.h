#pragma once
#include "Texture.h"
#include "CubeMap.h"

#include <string>
#include <vector>
#include <map>

namespace Sprocket {

class TextureManager
{
public:
    using TexMap = std::map<std::string, Texture>;
    using CubeMap = std::map<std::string, CubeMap>;

private:
    TexMap  d_loadedTextures;
    CubeMap d_loadedCubeMaps;

public:
    Texture GetTexture(const std::string& path);
        // Returns the texture in the specified path. If it has not
        // been loaded it will be loaded and stored internally.
};

}