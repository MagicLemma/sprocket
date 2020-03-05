#pragma once
#include "Graphics/Model.h"
#include "Graphics/Texture.h"

namespace Sprocket {

class Skybox
{
    TextureCubePtr d_texture;
    ModelPtr       d_model;

public:
    Skybox(TextureCubePtr texture);

    void bind() const;
    void unbind() const;

    // Getters / Setters
    ModelPtr model() const { return d_model; }
    TextureCubePtr texture() const { return d_texture; }
};

}