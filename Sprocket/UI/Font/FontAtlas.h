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

    int Fit(int index, int width, int height);

public:
    FontAtlas(int width, int height);

    Maths::ivec4 GetRegion(
        std::size_t width,
        std::size_t height,
        int padding
    );

    void SetRegion(
        const Maths::ivec4& region,
        const std::vector<unsigned char>& data
    );

    std::size_t Width() const { return d_texture.Width(); }
    std::size_t Height() const { return d_texture.Height(); }

    void Bind() const { d_texture.Bind(); }
    void Unbind() const { d_texture.Unbind(); }

    Texture GetAtlas() const { return d_texture; }
};

}

