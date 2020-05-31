#pragma once
#include "Model3D.h"
#include "Texture.h"

#include <string>
#include <vector>
#include <map>

namespace Sprocket {

class ModelManager
{
public:
    using Map = std::map<std::string, Model3D>;

private:
    Map d_loadedModels;

public:
    static Model3D LoadModel(const std::string& path);
        // Loads a model without caching it.

    Model3D LoadModel(const std::string& name,
                      const std::string& path);

    Model3D GetModel(const std::string& name) const;

    // Lower camel case to match STL iterators
    Map::iterator begin();
    Map::iterator end();
    Map::const_iterator cbegin() const;
    Map::const_iterator cend() const;
};

}