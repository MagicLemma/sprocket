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

    bool useAlbedoMap = false;
    bool useNormalMap = false;
    bool useMetallicMap = false;
    bool useRoughnessMap = false;

    Maths::vec3 albedo = {1.0f, 1.0f, 1.0f};
    float       metallic = 0.0f;  // AKA Reflectivity
    float       roughness = 1.0f; // AKA Shine Damper
    // If the normal map is not used, the "default" value used is
    // the normal in the Model VBO.
};

}