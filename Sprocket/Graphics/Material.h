#pragma once
#include "Texture.h"
#include "Maths.h"

#include <string>
#include <memory>

namespace Sprocket {

struct Material
{
    std::string name;
    std::string file;

    std::shared_ptr<Texture> albedoMap = std::shared_ptr<Texture>();
    std::shared_ptr<Texture> normalMap = std::shared_ptr<Texture>();
    std::shared_ptr<Texture> metallicMap = std::shared_ptr<Texture>();
    std::shared_ptr<Texture> roughnessMap = std::shared_ptr<Texture>();

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