#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <limits.h>
#include "texture-atlas.h"
#include "Maths.h"

#include <glad/glad.h>

namespace Sprocket {

FontAtlas::FontAtlas(std::size_t width, std::size_t height)
    : d_texture(width, height, Texture::Channels::RED)
{
    // We want a one pixel border around the whole atlas to avoid any
    // artefact when sampling texture
    Maths::ivec3 node;
    node.x = 1;
    node.y = 1;
    node.z = width - 2;

    d_nodes.push_back(node);
}

void FontAtlas::SetRegion(
    std::size_t x,
    std::size_t y,
    std::size_t width,
    std::size_t height,
    std::size_t stride,
    const unsigned char* data)
{
    assert(x > 0);
    assert(y > 0);
    assert(x < d_texture.Width() - 1);
    assert((x + width) <= d_texture.Width() - 1);
    assert(y < d_texture.Height() - 1);
    assert((y + height) <= d_texture.Height() - 1);

    // prevent copying data from undefined position
    // and prevent memcpy's undefined behavior when count is zero
    assert(height == 0 || (data != NULL && width > 0));

    d_texture.Bind();
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexSubImage2D(GL_TEXTURE_2D,
                    0, x, y, width, height, 
                    GL_RED, GL_UNSIGNED_BYTE, (void*)data);
    d_texture.Unbind();
}

int FontAtlas::Fit(
    std::size_t index,
    std::size_t width,
    std::size_t height)
{
    int x, y, width_left;
    size_t i;

    auto node = d_nodes[index];
    x = node.x;
    y = node.y;
    width_left = width;
    i = index;

    if ((x + width) > (d_texture.Width() - 1)) {
        return -1;
    }

    y = node.y;
    while (width_left > 0) {
        auto node2 = d_nodes[i];
        if (node2.y > y) {
            y = node2.y;
        }

        if ((y + height) > (d_texture.Height() - 1)) {
            return -1;
        }

        width_left -= node2.z;
        ++i;
    }
    return y;
}

Maths::ivec4 FontAtlas::GetRegion(
    std::size_t width,
    std::size_t height )
{
    Maths::ivec4 region{0, 0, width, height};

    std::size_t best_height = UINT_MAX;
    std::size_t best_width = UINT_MAX;

    int best_index  = -1;

    for (std::size_t i = 0; i < d_nodes.size(); ++i)
    {
        int y = Fit(i, width, height);
        if (y >= 0) {
            auto node = d_nodes[i];
            if ((y + height < best_height) ||
                ((y + height == best_height) &&
                 (node.z > 0 &&
                 (size_t)node.z < best_width)))
            {
                best_height = y + height;
                best_index = i;
                best_width = node.z;
                region.x = node.x;
                region.y = y;
            }
        }
    }

    if (best_index == -1) {
        region.x = -1;
        region.y = -1;
        region.z = 0; // width
        region.w = 0; // height
        return region;
    }

    Maths::ivec3 n;
    n.x = region.x;
    n.y = region.y + height;
    n.z = width;
    d_nodes.insert(d_nodes.begin() + best_index, n);

    for (std::size_t i = best_index + 1; i < d_nodes.size(); ++i) {
        auto& node = d_nodes[i];
        auto& prev = d_nodes[i-1];

        if (node.x < prev.x + prev.z) {
            int shrink = prev.x + prev.z - node.x;
            node.x += shrink;
            node.z -= shrink;
            if (node.z <= 0) {
                d_nodes.erase(d_nodes.begin() + i);
                --i;
            }
            else {
                break;
            }
        }
        else {
            break;
        }
    }
    
    // Merge
    for (std::size_t i = 0; i < d_nodes.size() - 1; ++i ) {
        auto& node = d_nodes[i];
        auto& next = d_nodes[i+1];
        if (node.y == next.y) {
            node.z += next.z;
            d_nodes.erase(d_nodes.begin() + i + 1);
            --i;
        }
    }

    return region;
}

}