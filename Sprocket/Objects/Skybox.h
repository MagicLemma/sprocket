#pragma once
#include "Model3D.h"
#include "CubeMap.h"

#include <array>

namespace Sprocket {

class Skybox
{
    Model3D d_model;
    CubeMap d_texture;

public:
    Skybox(const Model3D& model,
           const CubeMap& texture);

    void bind() const;
    void unbind() const;

    // Getters / Setters
    Model3D model() const { return d_model; }
    CubeMap texture() const { return d_texture; }
};

}