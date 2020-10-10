#pragma once
#include "Resources.h"
#include "Maths.h"

#include <memory>
#include <string>

namespace Sprocket {


class Texture
{
public:
    enum class Channels { RGBA, RED, DEPTH };

private:
    std::shared_ptr<TEX> d_texture;

    int d_width;
    int d_height;

    Channels d_channels = Channels::RGBA;
        // TODO: Generalise other constructors to expose this.

    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;

public:
    Texture(int width, int height, const unsigned char* data);
    Texture(int width, int height, const std::vector<unsigned char>& data);
    Texture(int width, int height, std::shared_ptr<TEX> texture);
    Texture(int width, int height, Channels channels = Channels::RGBA);
    Texture();

    static std::shared_ptr<Texture> FromFile(const std::string file);

    void Bind() const;
    void Unbind() const;

    int Width() const { return d_width; }
    int Height() const { return d_height; }
    float AspectRatio() const { return (float)d_width / (float)d_height; }

    // Standard texture builders
    static const Texture& White();

    void SetSubTexture(const Maths::ivec4& region,
                       const unsigned char* data);

    unsigned int Id() const;

    int GetChannels() const { return d_channels == Channels::RGBA ? 4 : 1; }

    bool operator==(const Texture& other) const;
};

}