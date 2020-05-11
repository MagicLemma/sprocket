#pragma once
#include "Model3D.h"
#include "Texture.h"

#include <string>
#include <vector>

namespace Sprocket {

// Expand this to include textures in the future
struct ModelLoaderData
{
    Model3D model;
};

class ModelLoader
{
public:
    std::vector<ModelLoaderData> loadModel(const std::string& path);
};

}