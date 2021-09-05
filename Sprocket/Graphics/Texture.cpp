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

texture_data texture_data::load(const std::string& file)
{
    texture_data td;
    unsigned char* d = stbi_load(file.c_str(), &td.width, &td.height, nullptr, 4);
    std::span<unsigned char> span_data{d, (std::size_t)(td.width * td.height * 4)};
    td.bytes = {span_data.begin(), span_data.end()};
    stbi_image_free(d);
    return td;
}

texture::texture(const texture_data& data)
    : d_id(0)
    , d_width(data.width)
    , d_height(data.height)
    , d_channels(spkt::texture_channels::RGBA)
{
    glCreateTextures(GL_TEXTURE_2D, 1, &d_id);
    SetTextureParameters(d_id);
    glTextureStorage2D(d_id, 1, GL_RGBA8, d_width, d_height);
    glTextureSubImage2D(d_id, 0, 0, 0, d_width, d_height, GL_RGBA, GL_UNSIGNED_BYTE, data.bytes.data());
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

std::unique_ptr<texture> texture::from_file(const std::string file)
{
    return std::make_unique<texture>(texture_data::load(file));
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
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4); // Reset back to initial value
}

}