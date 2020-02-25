#pragma once
#include "Graphics/GUI/Model2D.h"
#include "Graphics/Shader.h"

namespace Sprocket {

class Renderer2D
{
public:
    Renderer2D() {}
    
    void render(const Model2D& model, const Shader& shader);
};

}