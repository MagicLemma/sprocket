#pragma once

#include <string>

namespace Sprocket {

struct ModelComponent
{
    std::string model        = "";
    float       scale        = 1.0f;

    std::string texture      = "";    
    float       shineDamper  = 1.0f;
    float       reflectivity = 0.0f;
};

}