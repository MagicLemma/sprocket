#pragma once
#include <cstdlib>
#include <vector>

#include "Maths.h"
#include "Texture.h"

namespace Sprocket {

struct FontAtlas
{
    std::vector<Maths::ivec3> nodes;

    Texture texture;
};


std::shared_ptr<FontAtlas> texture_atlas_new(
    std::size_t width,
    std::size_t height
);

Maths::ivec4 texture_atlas_get_region(
    std::shared_ptr<FontAtlas> self,
    std::size_t width,
    std::size_t height
);

int texture_atlas_fit(
    std::shared_ptr<FontAtlas> self,
    std::size_t index,
    std::size_t width,
    std::size_t height
);

void texture_atlas_set_region(
    std::shared_ptr<FontAtlas> self,
    std::size_t x,
    std::size_t y,
    std::size_t width,
    std::size_t height,
    std::size_t stride,
    const unsigned char *data
);

}

