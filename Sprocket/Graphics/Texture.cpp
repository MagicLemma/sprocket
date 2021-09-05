#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <glad/glad.h>

#include <memory>
#include <span>

namespace spkt {
namespace {

void SetTextureParameters(std::uint32_t id)
{
    glTextureParameteri(id, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(id, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTextureParameteri(id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

}

texture_data::texture_data(const std::string& file)
{
    unsigned char* d = stbi_load(file.c_str(), &width, &height, &bpp, 4);
    std::span<unsigned char> span_data{d, (std::size_t)(width * height * 4)};
    data = {span_data.begin(), span_data.end()};
}

texture::texture(int width, int height, const unsigned char* data)
    : d_width(width)
    , d_height(height)
{
    glCreateTextures(GL_TEXTURE_2D, 1, &d_id);
    SetTextureParameters(d_id);
    glTextureStorage2D(d_id, 1, GL_RGBA8, width, height);
    glTextureSubImage2D(d_id, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
}

texture::texture(int width, int height, texture_channels channels)
    : d_width(width)
    , d_height(height)
    , d_channels(channels)
{
    resize(width, height);
}

texture::texture()
    : d_id(0)
    , d_width(0)
    , d_height(0)
{
}

texture::~texture()
{
    if (d_id > 0) { glDeleteTextures(1, &d_id); }
}

std::unique_ptr<texture> texture::from_data(const texture_data& data)
{
    return std::make_unique<texture>(data.width, data.height, data.data.data());
}

std::unique_ptr<texture> texture::from_file(const std::string file)
{
    return texture::from_data(texture_data(file));
}

void texture::resize(int width, int height)
{
    if (d_id) {
        glDeleteTextures(1, &d_id);
    }

    glCreateTextures(GL_TEXTURE_2D, 1, &d_id);
    SetTextureParameters(d_id);

    int ifmt, fmt;
    switch (d_channels) {
        case texture_channels::RGBA: {
            ifmt = GL_RGBA;
            fmt = GL_RGBA;
        } break;
        case texture_channels::RED: {
            ifmt = GL_RED;
            fmt = GL_RED;
        } break;
        case texture_channels::DEPTH: {
            ifmt = GL_DEPTH24_STENCIL8;
            fmt = GL_DEPTH_COMPONENT;
        } break;
    }

    glBindTexture(GL_TEXTURE_2D, d_id);
    glTexImage2D(GL_TEXTURE_2D, 0, ifmt, width, height, 0, fmt, GL_UNSIGNED_BYTE, nullptr);
    glBindTexture(GL_TEXTURE_2D, 0);

    d_width = width;
    d_height = height;
}

void texture::bind(int slot) const
{
    glBindTextureUnit(slot, d_id);
}

std::uint32_t texture::id() const
{
    return d_id;
}

bool texture::operator==(const texture& other) const
{
    return d_id == other.d_id;
}

void texture::set_subtexture(
    const glm::ivec4& region,
    const unsigned char* data)
{
    auto c = GL_RGBA;
    if (d_channels == texture_channels::RED) {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        c = GL_RED;
    }
    else if (d_channels == texture_channels::DEPTH) {
        c = GL_DEPTH_COMPONENT;
    }

    glTextureSubImage2D(
        d_id, 0, region.x, region.y, region.z, region.w, c, GL_UNSIGNED_BYTE, data
    );
}

}