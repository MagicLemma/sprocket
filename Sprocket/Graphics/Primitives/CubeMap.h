#pragma once
#include "Resources.h"

#include <array>

namespace Sprocket {

class CubeMap
{
    std::shared_ptr<TEX> d_texture;

    int d_width;
    int d_height;
    int d_bpp;

public:
    CubeMap(const std::array<std::string, 6>& faceFiles);

    void bind() const;
    void unbind() const;

    int width() const { return d_width; }
    int height() const { return d_height; }
    int bpp() const { return d_bpp; }
};

}