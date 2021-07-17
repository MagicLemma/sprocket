#pragma once
#include <glm/glm.hpp>

#include <string>
#include <memory>

namespace spkt {

struct Material
{
    static std::unique_ptr<Material> FromFile(const std::string& file);
    void Save() const;

    std::string name;
    std::string file;

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

}