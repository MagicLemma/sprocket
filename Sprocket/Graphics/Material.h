#pragma once
#include <glm/glm.hpp>

#include <string>

namespace spkt {

struct Material
{
    using data_type = Material; // Used in the asset manager to load these
    static Material load(std::string_view file);

    Material() = default;
    Material(const Material&) = default;

    std::string name;

    std::string albedoMap;
    std::string normalMap;
    std::string metallicMap;
    std::string roughnessMap;

    bool useAlbedoMap = false;
    bool useNormalMap = false;
    bool useMetallicMap = false;
    bool useRoughnessMap = false;

    glm::vec3 albedo = {1.0f, 1.0f, 1.0f};
    float     metallic = 0.0f;  // AKA Reflectivity
    float     roughness = 1.0f; // AKA Shine Damper
    // If the normal map is not used, the "default" value used is
    // the normal in the Model VBO.
};

void save_material(const std::string& file, const Material& material);

}