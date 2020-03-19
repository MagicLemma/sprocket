#include "Material.h"

namespace Sprocket {

Material::Material(const Texture& texture)
    : d_texture(texture)
    , d_shineDamper(1.0f)
    , d_reflectivity(0.0f)
{
}

void Material::bind() const
{
    d_texture.bind();
}

void Material::unbind() const
{
    d_texture.unbind();
}

}