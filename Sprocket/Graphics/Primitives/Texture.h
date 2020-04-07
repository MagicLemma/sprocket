#pragma once
#include "Resources.h"

#include <memory>
#include <string>

namespace Sprocket {

class Texture
{
    std::shared_ptr<TEX> d_texture;

    int d_width;
    int d_height;
    int d_bpp;

public:
    Texture(const std::string& pngFile);
    Texture(int width, int height, const std::vector<unsigned char>& data);
    Texture();

    void bind() const;
    void unbind() const;

    int width() const { return d_width; }
    int height() const { return d_height; }
    int bpp() const { return d_bpp; }

    // Standard texture builders
    static Texture white();
};

}