#pragma once
#include "Effect.h"

namespace Sprocket {

class GaussianVert : public Effect
{
    void setUniforms() const override;

public:
    GaussianVert(ResourceManager* resourceManager, int width, int height);
};

class GaussianHoriz : public Effect
{
    void setUniforms() const override;

public:
    GaussianHoriz(ResourceManager* resourceManager, int width, int height);
};
    
}