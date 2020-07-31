#pragma once
#include "Model3D.h"
#include "Texture.h"

#include <string>

namespace Sprocket {

struct ModelComponent
{
    std::string model;
    float       scale = 1.0f;

    Texture     texture;    
    float       shineDamper  = 1.0f;
    float       reflectivity = 0.0f;
};

}