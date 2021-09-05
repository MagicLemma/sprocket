#pragma once
#include <glm/glm.hpp>

#include <memory>
#include <string>

namespace spkt {

struct TextureData
{
    int width;
    int height;
    int bpp;
    unsigned char* data;

    TextureData(const TextureData&) = delete;
    TextureData& operator=(const TextureData&) = delete;

    TextureData(const std::string& file);
    ~TextureData();
};

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

    static std::unique_ptr<Texture> FromData(const TextureData& data);
    static std::unique_ptr<Texture> FromFile(const std::string file);

    void Bind(int slot) const;

    std::uint32_t Id() const;

    int Width() const { return d_width; }
    int Height() const { return d_height; }
    float AspectRatio() const { return (float)d_width / (float)d_height; }

    bool operator==(const Texture& other) const;

    // Mutable Texture Functions
    void SetSubTexture(const glm::ivec4& region, const unsigned char* data);
    void Resize(int width, int height);

};

}