#pragma once

namespace Sprocket {

class CubeMap
{
    unsigned int d_textureId;

    int d_width;
    int d_height;
    int d_bpp;

public:
    CubeMap(unsigned int textureId, int width, int height, int bpp);

    void bind() const;
    void unbind() const;

    int width() const { return d_width; }
    int height() const { return d_height; }
    int bpp() const { return d_bpp; }
};

}