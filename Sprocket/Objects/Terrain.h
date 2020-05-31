#pragma once
#include "Model3D.h"
#include "Material.h"
#include "Maths.h"

namespace Sprocket {

class Terrain
{
    Material    d_material;

    Maths::vec3 d_position;

    int         d_edge;
    float       d_distance;
    Model3D     d_model;

public:
    Terrain(const Material& material,
            const Maths::vec3& postition = Maths::vec3{0.0f});

    void Bind() const;
    void Unbind() const;

    // Getters / Setters
    Model3D GetModel() const { return d_model; }
    Material GetMaterial() const { return d_material; }

    Maths::vec3 Position() const { return d_position; }
};

}