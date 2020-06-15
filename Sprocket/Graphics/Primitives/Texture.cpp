#include "Texture.h"
#include "Log.h"

#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace Sprocket {

Texture::Texture(const std::string& pngFile, bool flip)
    : d_texture(std::make_shared<TEX>())
{
    glBindTexture(GL_TEXTURE_2D, d_texture->Value());

    SPKT_LOG_INFO("Loading texture '{}'", pngFile);
    int bpp;
    stbi_set_flip_vertically_on_load(flip);
    unsigned char* data = stbi_load(
        pngFile.c_str(),
        &d_width, &d_height, &bpp, 4);

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

Texture::Texture(int width, int height, const std::vector<unsigned char>& data)
    : d_texture(std::make_shared<TEX>())
    , d_width(width)
    , d_height(height)
{
    glBindTexture(GL_TEXTURE_2D, d_texture->Value());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D,
                 0, GL_RGBA, d_width, d_height,
                 0, GL_RGBA, GL_UNSIGNED_BYTE, data.data());

    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::Texture(int width, int height, unsigned char* data)
    : d_texture(std::make_shared<TEX>())
    , d_width(width)
    , d_height(height)
{
    glBindTexture(GL_TEXTURE_2D, d_texture->Value());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D,
                 0, GL_RGBA, d_width, d_height,
                 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::Texture()
    : d_texture(Texture::White().d_texture)
    , d_width(Texture::White().d_width)
    , d_height(Texture::White().d_height)
{
}

void Texture::Bind() const
{
    glBindTexture(GL_TEXTURE_2D, d_texture->Value());
}

void Texture::Unbind() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture Texture::White()
{
    static const Texture white(1, 1, {0xff, 0xff, 0xff, 0x55});
    return white;
}

unsigned int Texture::Id() const
{
    return d_texture->Value();
}

bool Texture::operator==(const Texture& other) const
{
    return d_texture->Value() == other.d_texture->Value();
}

}