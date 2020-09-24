#pragma once
#include "Model3D.h"

#include <string>
#include <vector>
#include <map>
#include <memory>

namespace Sprocket {

class ModelManager
{
public:
    using Map = std::map<std::string, std::shared_ptr<Mesh>>;

private:
    Map d_loadedModels;

public:
    static std::shared_ptr<Mesh> LoadModel(const std::string& path);
        // Loads a model without caching it.

    std::shared_ptr<Mesh> GetModel(const std::string& path);
        // Returns the model in the specified path. If it has not beed loaded
        // it will be loaded and stored internally.

    // Lower camel case to match STL iterators
    Map::iterator begin();
    Map::iterator end();
    Map::const_iterator cbegin() const;
    Map::const_iterator cend() const;
};

}