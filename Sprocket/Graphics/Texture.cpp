#include "Texture.h"
#include "Log.h"
#include "Maths.h"

#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace Sprocket {
namespace {

void SetTextureParameters(u32 id)
{
    glTextureParameteri(id, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(id, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTextureParameteri(id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

}

TextureData::TextureData(const std::string& file)
{
    data = stbi_load(file.c_str(), &width, &height, &bpp, 4);
}

TextureData::~TextureData()
{
    stbi_image_free(data);
}

Texture::Texture(int width, int height, const unsigned char* data)
    : d_width(width)
    , d_height(height)
{
    glCreateTextures(GL_TEXTURE_2D, 1, &d_id);
    SetTextureParameters(d_id);
    glTextureStorage2D(d_id, 1, GL_RGBA8, width, height);
    glTextureSubImage2D(d_id, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
}

Texture::Texture(int width, int height, Channels channels)
    : d_width(width)
    , d_height(height)
    , d_channels(channels)
{
    glCreateTextures(GL_TEXTURE_2D, 1, &d_id);
    SetTextureParameters(d_id);
    Resize(width, height);
}

Texture::Texture()
    : d_id(0)
    , d_width(0)
    , d_height(0)
{
}

Texture::~Texture()
{
    if (d_id > 0) { glDeleteTextures(1, &d_id); }
}

std::shared_ptr<Texture> Texture::FromData(const TextureData& data)
{
    return std::make_shared<Texture>(data.width, data.height, data.data);
}

std::shared_ptr<Texture> Texture::FromFile(const std::string file)
{
    return Texture::FromData(TextureData(file));
}

void Texture::Resize(int width, int height)
{
    int ifmt, fmt;
    switch (d_channels) {
        case Channels::RGBA: {
            ifmt = GL_RGBA;
            fmt = GL_RGBA;
        } break;
        case Channels::RED: {
            ifmt = GL_RED;
            fmt = GL_RED;
        } break;
        case Channels::DEPTH: {
            ifmt = GL_DEPTH_COMPONENT16;
            fmt = GL_DEPTH_COMPONENT;
        } break;
    }

    glBindTexture(GL_TEXTURE_2D, d_id);
    glTexImage2D(GL_TEXTURE_2D, 0, ifmt, width, height, 0, fmt, GL_UNSIGNED_BYTE, nullptr);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Bind(int slot) const
{
    glBindTextureUnit(slot, d_id);
}

u32 Texture::Id() const
{
    return d_id;
}

bool Texture::operator==(const Texture& other) const
{
    return d_id == other.d_id;
}

void Texture::SetSubTexture(
    const glm::ivec4& region,
    const unsigned char* data)
{
    auto c = GL_RGBA;
    if (d_channels == Channels::RED) {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        c = GL_RED;
    }
    else if (d_channels == Channels::DEPTH) {
        c = GL_DEPTH_COMPONENT;
    }

    glTextureSubImage2D(
        d_id, 0,
        region.x, region.y, region.z, region.w,
        c, GL_UNSIGNED_BYTE, data
    );
}

}