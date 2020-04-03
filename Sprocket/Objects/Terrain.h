#pragma once
#include "Model3D.h"
#include "Material.h"
#include "Maths.h"
#include "ResourceManager.h"

namespace Sprocket {

class Terrain
{
    Material     d_material;

    Maths::vec3 d_position;

    int         d_edge;
    float       d_distance;
    Model3D     d_model;

public:
    Terrain(ResourceManager* resourceManager,
            const Material& material,
            const Maths::vec3& postition = Maths::vec3{0.0f});

    void bind() const;
    void unbind() const;

    // Getters / Setters
    Model3D model() const { return d_model; }
    Material material() const { return d_material; }

    Maths::vec3 position() const { return d_position; }
};

}