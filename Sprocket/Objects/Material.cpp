#include "Material.h"

namespace Sprocket {

Material::Material(const Texture& texture)
    : d_texture(texture)
    , d_shineDamper(1.0f)
    , d_reflectivity(0.0f)
{
}

Material::Material()
    : d_texture()
    , d_shineDamper(1.0f)
    , d_reflectivity(0.0f)
{
}


void Material::bind(int index) const
{
    d_texture.bind(index);
}

void Material::unbind() const
{
    d_texture.unbind();
}

bool Material::operator==(const Material& other) const
{
    return d_texture == other.d_texture
        && d_shineDamper == other.d_shineDamper
        && d_reflectivity == other.d_reflectivity;
}

}