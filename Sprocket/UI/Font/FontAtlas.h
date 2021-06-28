#pragma once
#include <cstdlib>
#include <vector>

#include "Maths.h"
#include "Texture.h"

namespace spkt {

class FontAtlas
{
    std::vector<glm::ivec3> d_nodes;
    std::unique_ptr<Texture> d_texture;

    int Fit(int index, int width, int height);

public:
    FontAtlas(int width, int height);

    glm::ivec4 GetRegion(std::size_t width, std::size_t height);
    void SetRegion(const glm::ivec4& region, const unsigned char* data);

    std::size_t Width() const { return d_texture->Width(); }
    std::size_t Height() const { return d_texture->Height(); }

    void Bind(int slot) const { d_texture->Bind(slot); }

    Texture* GetAtlas() const { return d_texture.get(); }
};

}

