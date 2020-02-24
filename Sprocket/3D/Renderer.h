#pragma once
#include "3D/Entity.h"
#include "3D/Camera.h"
#include "3D/Shader.h"
#include "3D/Light.h"

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