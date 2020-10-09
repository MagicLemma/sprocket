#include "Texture.h"
#include "Log.h"
#include "Maths.h"

#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace Sprocket {

Texture::Texture(int width, int height, const unsigned char* data)
    : d_texture(std::make_shared<TEX>())
    , d_width(width)
    , d_height(height)
{
    assert(width > 0);
    assert(height > 0);

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

Texture::Texture(int width, int height, const std::vector<unsigned char>& data)
    : Texture(width, height, data.data())
{
}

Texture::Texture(int width, int height, std::shared_ptr<TEX> texture)
    : d_texture(texture)
    , d_width(width)
    , d_height(height)
{
}

Texture::Texture(int width, int height, Channels channels)
    : d_texture(std::make_shared<TEX>())
    , d_width(width)
    , d_height(height)
    , d_channels(channels)
{
    auto c = channels == Channels::RGBA ? GL_RGBA : GL_RED;

    glBindTexture(GL_TEXTURE_2D, d_texture->Value());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    if (channels == Channels::RED) {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    }

    glTexImage2D(GL_TEXTURE_2D,
                 0, c, width, height,
                 0, c, GL_UNSIGNED_BYTE, nullptr);
                 
    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::Texture()
    : d_texture(Texture::White().d_texture)
    , d_width(Texture::White().d_width)
    , d_height(Texture::White().d_height)
{
}

std::shared_ptr<Texture> Texture::FromFile(const std::string file)
{
    SPKT_LOG_INFO("Loading texture '{}'", file);
    int width, height, bpp;
    unsigned char* data = stbi_load(file.c_str(), &width, &height, &bpp, 4);
    return std::make_shared<Texture>(width, height, data);
}

void Texture::Bind() const
{
    glBindTexture(GL_TEXTURE_2D, d_texture->Value());
}

void Texture::Unbind() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

const Texture& Texture::White()
{
    static const Texture white(1, 1, {0xff, 0xff, 0xff, 0xff});
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

void Texture::SetSubTexture(
    const Maths::ivec4& region,
    const unsigned char* data)
{
    Bind();

    if (d_channels == Channels::RED) {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    }

    auto c = d_channels == Channels::RGBA ? GL_RGBA : GL_RED;
    glTexSubImage2D(GL_TEXTURE_2D,
                    0, region.x, region.y, region.z, region.w, 
                    c, GL_UNSIGNED_BYTE, (void*)data);

    Unbind();
}

}