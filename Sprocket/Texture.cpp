#include "Texture.h"

#include <glad/glad.h>

namespace Sprocket {

Texture::Texture(unsigned int textureId)
    : d_textureId(textureId)
{
}

void Texture::bind() const
{
    glBindTexture(GL_TEXTURE_2D, d_textureId);
}

void Texture::unbind() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}
    
}