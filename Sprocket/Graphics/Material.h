#pragma once
#include <glm/glm.hpp>

#include <string>

namespace spkt {

struct Material
{
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
    float     metallic = 0.0f;
    float     roughness = 1.0f;

    static Material load(std::string_view file);
};

void save_material(const std::string& file, const Material& material);

}