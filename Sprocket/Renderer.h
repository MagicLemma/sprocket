#pragma once
#include "Entity.h"
#include "Camera.h"
#include "Shader.h"
#include "Light.h"

#include <vector>

namespace Sprocket {

class Renderer
{
public:
    Renderer();
    
    void render(const Entity& entity,
                const std::vector<Light>& lights,
                const Camera& camera,
                const Shader& shader);
};

}