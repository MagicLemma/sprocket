#pragma once
#include "Entity.h"
#include "Camera.h"
#include "Shader.h"

namespace Sprocket {

class Renderer
{
public:
    void render(const Entity& entity,
                const Camera& camera,
                const Shader& shader);
};

}