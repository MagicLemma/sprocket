#pragma once
#include <cstdlib>
#include <memory>
#include <vector>

#include "Maths.h"
#include "Texture.h"

namespace Sprocket {

struct texture_atlas_t
{
    std::vector<std::shared_ptr<Sprocket::Maths::ivec3>> nodes;

    Texture texture;
};


std::shared_ptr<texture_atlas_t> texture_atlas_new(
    std::size_t width,
    std::size_t height
);

Maths::ivec4 texture_atlas_get_region(
    std::shared_ptr<texture_atlas_t> self,
    std::size_t width,
    std::size_t height
);

int texture_atlas_fit(
    std::shared_ptr<texture_atlas_t> self,
    std::size_t index,
    std::size_t width,
    std::size_t height
);

void texture_atlas_set_region(
    std::shared_ptr<texture_atlas_t> self,
    std::size_t x,
    std::size_t y,
    std::size_t width,
    std::size_t height,
    std::size_t stride,
    const unsigned char *data
);

}

