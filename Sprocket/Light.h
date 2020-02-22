#pragma once
#include <glm/glm.hpp>

namespace Sprocket {

struct Light
{
    glm::vec3 position;
    glm::vec3 colour;
    glm::vec3 attenuation;
};

}