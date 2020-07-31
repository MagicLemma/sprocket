#include "TextureManager.h"
#include "Log.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Sprocket {

Texture TextureManager::GetTexture(const std::string& path)
{
    if (path == "") { return Texture::White(); }
    
    auto it = d_loadedTextures.find(path);
    if (it != d_loadedTextures.end()) {
        return it->second;
    }
    Texture model = Texture(path);
    d_loadedTextures.emplace(path, model);
    return model;   
}

}