#pragma once
#include "Resources.h"

#include <memory>
#include <string>

namespace Sprocket {

enum class TextureType
{
    DIFFUSE,
    SPECULAR,
    NORMAL
};

class Texture
{
    std::shared_ptr<TEX> d_texture;

    int d_width;
    int d_height;
    int d_bpp;

    TextureType d_type;

public:
    Texture(const std::string& pngFile,
            TextureType type = TextureType::DIFFUSE);

    Texture(int width, int height, const std::vector<unsigned char>& data,
            TextureType type = TextureType::DIFFUSE);

    Texture();

    void bind() const;
    void unbind() const;

    int width() const { return d_width; }
    int height() const { return d_height; }
    int bpp() const { return d_bpp; }

    TextureType type() const { return d_type; }

    // Standard texture builders
    static Texture white();

    bool operator==(const Texture& other) const;
};

}