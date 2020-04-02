#pragma once
#include "Model3D.h"

#include <unordered_map>
#include <string>

namespace Sprocket {

class ResourceManager
{
    std::unordered_map<std::string, Model3D> d_models;

public:
    Model3D loadModel3D(const std::string& name, const std::string& objFile);
    Model3D getModel3D(const std::string& name) const;
};

}