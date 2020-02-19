#pragma once
#include "Models/TexturedModel.h"

namespace Sprocket {

class Renderer
{
public:
    void render(const TexturedModel& texturedModel);
};

}