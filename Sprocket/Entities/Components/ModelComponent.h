#pragma once
#include "Model3D.h"
#include "Material.h"

#include <string>

namespace Sprocket {

struct ModelComponent
{
    std::string model;
    Material    material;
    float       scale = 1.0f;  // Affects the size of the model.
};

}