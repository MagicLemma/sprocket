#pragma once
#include <glm/glm.hpp>

#include <memory>
#include <vector>
#include <string>

namespace spkt {

struct texture_data
{
    int width;
    int height;
    std::vector<unsigned char> bytes;

    static texture_data load(const std::string& file);
};

enum class texture_channels
{
    RGBA,
    RED,
    DEPTH
};

class texture
{
    std::uint32_t d_id;

    int d_width;
    int d_height;

    texture_channels d_channels;

    texture(const texture&) = delete;
    texture& operator=(const texture&) = delete;

public:
    texture(const texture_data& data);
    texture(int width, int height, texture_channels channels);
    texture();
    ~texture();

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