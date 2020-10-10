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
    std::uint32_t d_id;

    int d_width;
    int d_height;

    Channels d_channels;

    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;

public:
    Texture(int width, int height, const unsigned char* data);
    Texture(int width, int height, Channels channels = Channels::RGBA);
    Texture();
    ~Texture();

    static std::shared_ptr<Texture> FromFile(const std::string file);

    void Bind() const;
    void Unbind() const;

    unsigned int Id() const;

    int Width() const { return d_width; }
    int Height() const { return d_height; }
    float AspectRatio() const { return (float)d_width / (float)d_height; }

    int GetChannels() const { return d_channels == Channels::RGBA ? 4 : 1; }
    bool operator==(const Texture& other) const;

    // Mutable Texture Functions
    void SetSubTexture(const Maths::ivec4& region, const unsigned char* data);
    void Resize(int width, int height);

};

}