#pragma once
#include <Sprocket/Graphics/PostProcessing/Effect.h>

namespace spkt {

class GaussianVert : public Effect
{
public:
    GaussianVert(int width, int height);
};

class GaussianHoriz : public Effect
{
public:
    GaussianHoriz(int width, int height);
};
    
}