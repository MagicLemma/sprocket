#pragma once
#include "RawModel.h"
#include "Texture.h"

namespace Sprocket {

class Renderer
{
public:
    void render(const RawModel& rawModel, const Texture& texture);
};

}