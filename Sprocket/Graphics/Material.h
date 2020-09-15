#pragma once
#include "Texture.h"
#include "Maths.h"

#include <string>
#include <memory>

namespace Sprocket {

struct Material
{
    std::string file;

    Texture albedoMap;
    Texture normalMap;
    Texture metallicMap;
    Texture roughnessMap;

    bool useAlbedoMap;
    bool useNormalMap;
    bool useMetallicMap;
    bool useRoughnessMap;

    Maths::vec3 albedo;
    float       metallic;  // AKA Reflectivity
    float       roughness; // AKA Shine Damper
    // If the normal map is not used, the "default" value used is
    // the normal in the Model VBO.
};

}