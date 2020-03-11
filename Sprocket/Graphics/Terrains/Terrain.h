#pragma once
#include "Graphics/Modelling/Model3D.h"
#include "Graphics/Texture.h"
#include "Utility/Maths.h"

namespace Sprocket {

class Terrain
{
    Texture     d_texture;

    Maths::vec3 d_position;

    int         d_edge;
    float       d_distance;
    Model3D     d_model;

public:
    Terrain(const Texture& texture,
            const Maths::vec3& postition = Maths::vec3{0.0f});

    void bind() const;
    void unbind() const;

    // Getters / Setters
    Model3D model() const { return d_model; }
    Texture texture() const { return d_texture; }

    Maths::vec3 position() const { return d_position; }
};

}