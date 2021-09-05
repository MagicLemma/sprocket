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

class texture
{
public:
    enum class Channels { RGBA, RED, DEPTH };

private:
    std::uint32_t d_id;

    int d_width;
    int d_height;

    Channels d_channels;

    texture(const texture&) = delete;
    texture& operator=(const texture&) = delete;

public:
    texture(int width, int height, const unsigned char* data);
    texture(int width, int height, Channels channels = Channels::RGBA);
    texture();
    ~texture();

    static std::unique_ptr<texture> from_data(const TextureData& data);
    static std::unique_ptr<texture> from_file(const std::string file);

    void bind(int slot) const;

    std::uint32_t id() const;

    int width() const { return d_width; }
    int height() const { return d_height; }
    float aspect_ratio() const { return (float)d_width / (float)d_height; }

    bool operator==(const texture& other) const;

    // Mutable Texture Functions
    void set_subtexture(const glm::ivec4& region, const unsigned char* data);
    void resize(int width, int height);

};

}