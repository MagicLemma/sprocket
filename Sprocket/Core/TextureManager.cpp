#include "TextureManager.h"
#include "Log.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Sprocket {

TextureManager::TextureManager()
    : d_default(std::make_shared<Texture>())
{
}

std::shared_ptr<Texture> TextureManager::GetTexture(const std::string& path)
{
    if (path == "") { return d_default; }
    
    auto it = d_loadedTextures.find(path);
    if (it != d_loadedTextures.end()) {
        return it->second;
    }
    auto texture = std::make_shared<Texture>(path);
    d_loadedTextures.emplace(path, texture);
    return texture;   
}

}