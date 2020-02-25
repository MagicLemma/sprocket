#pragma once
#include "Graphics/GUI/Model2D.h"
#include "Graphics/GUI/Shader2D.h"

namespace Sprocket {

class Renderer2D
{
public:
    Renderer2D() {}
    
    void render(const Model2D& model, const Shader2D& shader);
};

}