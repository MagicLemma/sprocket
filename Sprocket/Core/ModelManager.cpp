#include "ModelManager.h"
#include "Log.h"

namespace Sprocket {

ModelManager::ModelManager()
    : d_default(std::make_shared<Mesh>())
{}

std::shared_ptr<Mesh> ModelManager::GetModel(const std::string& path)
{
    if (path == "") { return d_default; }
    
    if (auto it = d_loadedModels.find(path); it != d_loadedModels.end()) {
        return it->second;
    }
    auto model = Mesh::FromFile(path);
    d_loadedModels.emplace(path, model);
    return model;   
}

ModelManager::Map::iterator ModelManager::begin()
{
    return d_loadedModels.begin();
}

ModelManager::Map::iterator ModelManager::end()
{
    return d_loadedModels.end();
}

ModelManager::Map::const_iterator ModelManager::cbegin() const
{
    return d_loadedModels.cbegin();
}

ModelManager::Map::const_iterator ModelManager::cend() const
{
    return d_loadedModels.cend();
}


}