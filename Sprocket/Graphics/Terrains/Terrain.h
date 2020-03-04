#pragma once
#include "Graphics/Model.h"
#include "Graphics/Texture.h"
#include "Utility/Maths.h"

namespace Sprocket {

class Terrain
{
    ModelPtr    d_model;
    TexturePtr  d_texture;

    Maths::vec3 d_position;

    int         d_edge;
    float       d_distance;

public:
    Terrain(TexturePtr texture);

    void bind() const;
    void unbind() const;

    // Getters / Setters
    ModelPtr model() const { return d_model; }
    TexturePtr texture() const { return d_texture; }

    
};

}