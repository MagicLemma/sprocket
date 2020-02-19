#pragma once
#include "Entity.h"
#include "Shader.h"

namespace Sprocket {

class Renderer
{
public:
    void render(const Entity& entity, const Shader& shader);
};

}