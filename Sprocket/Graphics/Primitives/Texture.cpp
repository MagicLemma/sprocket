#include "Texture.h"
#include "Loader.h"

#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace Sprocket {

Texture::Texture(unsigned int textureId, int width, int height, int bpp)
    : d_textureId(textureId)
    , d_width(width)
    , d_height(height)
    , d_bpp(bpp)
{
}

Texture::Texture(const std::string& pngFile)
    : d_textureId(Loader::createTEX(Loader::TextureType::FLAT))
    , d_width(0)
    , d_height(0)
    , d_bpp(0)
{
    stbi_set_flip_vertically_on_load(1);
    unsigned char* data = stbi_load(pngFile.c_str(), &d_width, &d_height, &d_bpp, 4);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D,
                 0, GL_RGBA8, d_width, d_height,
                 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    
    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);
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