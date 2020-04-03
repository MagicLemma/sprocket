#include "Skybox.h"

namespace Sprocket {

Skybox::Skybox(const Model3D& model,
               const CubeMap& texture)
    : d_model(model)
    , d_texture(texture)
{   
}

void Skybox::bind() const
{
    d_model.bind();
    d_texture.bind();
}

void Skybox::unbind() const
{
    d_texture.unbind();
    d_model.unbind();
}

}