#pragma once
#include <cstdlib>
#include <vector>

#include "Maths.h"
#include "Texture.h"

namespace Sprocket {

class FontAtlas
{
    std::vector<Maths::ivec3> d_nodes;
    Texture d_texture;

    int Fit(std::size_t index, std::size_t width, std::size_t height);

public:
    FontAtlas(std::size_t width, std::size_t height);

    Maths::ivec4 GetRegion(std::size_t width, std::size_t height);

    void SetRegion(
        std::size_t x,
        std::size_t y,
        std::size_t width,
        std::size_t height,
        std::size_t stride,
        const unsigned char *data
    );

    std::size_t Width() const { return d_texture.Width(); }
    std::size_t Height() const { return d_texture.Height(); }

    void Bind() const { d_texture.Bind(); }
    void Unbind() const { d_texture.Unbind(); }
};

}

