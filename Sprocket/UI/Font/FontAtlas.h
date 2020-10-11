#pragma once
#include <cstdlib>
#include <vector>

#include "Maths.h"
#include "Texture.h"

namespace Sprocket {

class FontAtlas
{
    std::vector<Maths::ivec3> d_nodes;
    std::shared_ptr<Texture> d_texture;

    int Fit(int index, int width, int height);

public:
    FontAtlas(int width, int height);

    Maths::ivec4 GetRegion(std::size_t width, std::size_t height);

    void SetRegion(
        const Maths::ivec4& region,
        const unsigned char* data
    );

    std::size_t Width() const { return d_texture->Width(); }
    std::size_t Height() const { return d_texture->Height(); }

    void Bind(int slot) const { d_texture->Bind(slot); }
    void Unbind() const { d_texture->Unbind(); }

    std::shared_ptr<Texture> GetAtlas() const { return d_texture; }
};

}

