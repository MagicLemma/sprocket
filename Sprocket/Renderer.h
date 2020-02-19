#pragma once
#include "Model.h"
#include "Texture.h"

namespace Sprocket {

class Renderer
{
public:
    void render(const Model& rawModel, const Texture& texture);
};

}