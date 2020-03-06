#pragma once
#include "Graphics/Modelling/ModelSkybox.h"
#include "Graphics/Texture.h"

namespace Sprocket {

VertexSkyboxBuffer getCubeBuffer();

class Skybox
{
    std::shared_ptr<ModelSkybox> d_model;
    TextureCubePtr d_texture;

public:
    Skybox(TextureCubePtr texture);

    void bind() const;
    void unbind() const;

    // Getters / Setters
    std::shared_ptr<ModelSkybox> model() const { return d_model; }
    TextureCubePtr texture() const { return d_texture; }
};

}