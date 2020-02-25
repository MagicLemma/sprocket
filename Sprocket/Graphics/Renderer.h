#pragma once
#include "Graphics/Entity.h"
#include "Graphics/Camera.h"
#include "Graphics/Shader.h"
#include "Graphics/Light.h"

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