#pragma once
#include "Model3D.h"
#include "Material.h"

namespace Sprocket {

struct ModelComponent
{
    Model3D  model;
    Material material;
    float    scale = 1.0f;  // Affects the size of the model.
};

}