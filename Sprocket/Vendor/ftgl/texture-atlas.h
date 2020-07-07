#pragma once

#include <stdlib.h>
#include <memory>
#include <vector>

#include "vector.h"
#include "vec234.h"

namespace ftgl {

struct texture_atlas_t
{
    std::vector<std::shared_ptr<ivec3>> nodes;

    size_t width;
    size_t height;
    size_t depth;

    size_t used;

    unsigned int id;

    std::vector<unsigned char> data;
};


std::shared_ptr<texture_atlas_t> texture_atlas_new(
    const size_t width,
    const size_t height,
    const size_t depth
);

ivec4 texture_atlas_get_region(
    std::shared_ptr<texture_atlas_t> self,
    const size_t width,
    const size_t height
);

void texture_atlas_set_region(
    std::shared_ptr<texture_atlas_t> self,
    const size_t x,
    const size_t y,
    const size_t width,
    const size_t height,
    const unsigned char *data,
    const size_t stride
);

}

