#pragma once
#include <memory>
#include <string>

namespace Sprocket {

class Texture
{
    unsigned int d_textureId;

    int d_width;
    int d_height;
    int d_bpp;

public:
    Texture(unsigned int textureId, int width, int height, int bpp);
    Texture(const std::string& pngFile);

    void bind() const;
    void unbind() const;

    int width() const { return d_width; }
    int height() const { return d_height; }
    int bpp() const { return d_bpp; }

    static Texture& empty();
};

}