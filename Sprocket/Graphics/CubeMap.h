#pragma once
#include "Resources.h"

#include <array>

namespace Sprocket {

class CubeMap
{
    std::shared_ptr<TEX> d_texture;

    int d_width;
    int d_height;

public:
    CubeMap(const std::array<std::string, 6>& faceFiles);

    void Bind() const;
    void Unbind() const;

    int Width() const { return d_width; }
    int Height() const { return d_height; }
};

}