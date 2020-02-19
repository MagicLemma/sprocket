#pragma once
#include <string>

namespace Sprocket {

class Texture
{
    std::string    d_filePath;
    unsigned char* d_textureData;

    int            d_width;
    int            d_height;
    int            d_bpp;

public:
    Texture(const std::string& path);
    ~Texture();

    int width() const { return d_width; }
    int height() const { return d_height; }
    int bpp() const { return d_bpp; }

    unsigned char* data() const { return d_textureData; }
};

}