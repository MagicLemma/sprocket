#include "Texture.h"
#include "Log.h"
#include "Maths.h"

#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace Sprocket {

Texture::Texture(int width, int height, const unsigned char* data)
    : d_width(width)
    , d_height(height)
{
    assert(width > 0);
    assert(height > 0);

    glGenTextures(1, &d_id);
    glBindTexture(GL_TEXTURE_2D, d_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D,
                 0, GL_RGBA, d_width, d_height,
                 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::Texture(int width, int height, Channels channels)
    : d_width(width)
    , d_height(height)
    , d_channels(channels)
{
    auto c = channels == Channels::RGBA ? GL_RGBA : GL_RED;

    glGenTextures(1, &d_id);
    glBindTexture(GL_TEXTURE_2D, d_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    if (channels == Channels::RED) {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    }

    if (channels == Channels::DEPTH) {
        glTexImage2D(GL_TEXTURE_2D,
                    0, GL_DEPTH_COMPONENT16, width, height,
                    0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, nullptr);
    }
    else {
        glTexImage2D(GL_TEXTURE_2D,
                    0, c, width, height,
                    0, c, GL_UNSIGNED_BYTE, nullptr);
    }
                 
    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::Texture()
    : d_id(0)
    , d_width(0)
    , d_height(0)
{
}

Texture::~Texture()
{
    if (d_id > 0) {
        glDeleteTextures(1, &d_id);
    }
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
    glBindTexture(GL_TEXTURE_2D, d_id);
}

void Texture::Unbind() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

const Texture& Texture::White()
{
    auto GetData = []() {
        std::vector<unsigned char> data;
        data.push_back(0xff);
        data.push_back(0xff);
        data.push_back(0xff);
        data.push_back(0xff);
        return data;
    };
    static const Texture white(1, 1, GetData().data());
    return white;
}

unsigned int Texture::Id() const
{
    return d_id;
}

bool Texture::operator==(const Texture& other) const
{
    return d_id == other.d_id;
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