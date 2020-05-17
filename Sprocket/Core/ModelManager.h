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
    static Model3D loadModel(const std::string& path);
        // Loads a model without caching it.

    Model3D loadModel(const std::string& name,
                      const std::string& path);

    Model3D getModel(const std::string& name) const;

    Map::iterator begin();
    Map::iterator end();

    Map::const_iterator cbegin() const;
    Map::const_iterator cend() const;
};

}