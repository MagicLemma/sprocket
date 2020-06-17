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

public:
    Texture(const std::string& pngFile, bool flip = true);
    Texture(int width, int height, const std::vector<unsigned char>& data);
    Texture(int width, int height, unsigned char* data);
    Texture(int width, int height, std::shared_ptr<TEX> texture);
    Texture();

    void Bind() const;
    void Unbind() const;

    int Width() const { return d_width; }
    int Height() const { return d_height; }

    // Standard texture builders
    static Texture White();

    unsigned int Id() const;

    bool operator==(const Texture& other) const;
};

}