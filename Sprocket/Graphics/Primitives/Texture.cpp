#include "Texture.h"

#include <glad/glad.h>

namespace Sprocket {

Texture::Texture(unsigned int textureId, int width, int height, int bpp)
    : d_textureId(textureId)
    , d_width(width)
    , d_height(height)
    , d_bpp(bpp)
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