#include "3D/Texture.h"

#include <glad/glad.h>

namespace Sprocket {

Texture::Texture(unsigned int textureId)
    : d_textureId(textureId)
    , d_shineDamper(1.0f)
    , d_reflectivity(0.0f)
{
}

void Texture::bind() const
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, d_textureId);
}

void Texture::unbind() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}
    
}