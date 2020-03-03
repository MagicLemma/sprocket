#pragma once
#include "Graphics/Model.h"
#include "Graphics/Texture.h"

namespace Sprocket {

class Terrain
{
    int   d_edge;
    float d_distance;

    ModelPtr d_model;
    TexturePtr d_texture;

public:
    Terrain(float gridX, float gridZ, TexturePtr texture);

    ModelPtr model() const { return d_model; }
    TexturePtr texture() const { return d_texture; }
};

}