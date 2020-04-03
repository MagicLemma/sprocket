#pragma once
#include "Effect.h"

namespace Sprocket {

class Negative : public Effect
{
public:
    Negative(ResourceManager* resourceManager,
             int width, int height);
};
    
}