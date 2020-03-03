#pragma once
#include "Graphics/Model.h"
#include "Graphics/Texture.h"

namespace Sprocket {

class Terrain
{
    float d_size;
    int   d_vertexCount;

    float d_x;
    float d_z;

    Model d_model;
    Texture d_texture;

public:
    Terrain(float gridX, float gridZ, const Model& model, const Texture& texture);
};

}