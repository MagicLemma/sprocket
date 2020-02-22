#pragma once
#include "Entity.h"
#include "Camera.h"
#include "Shader.h"
#include "Light.h"

namespace Sprocket {

class Renderer
{
public:
    Renderer();
    
    void render(const Entity& entity,
                const Light& light,
                const Camera& camera,
                const Shader& shader);
};

}