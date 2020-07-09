#include "FontAtlas.h"

#include <cstdlib>
#include <cassert>

#include "Maths.h"

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
    const Maths::ivec4& region,
    const std::vector<unsigned char>& data)
{
    assert(region.x > 0);
    assert(region.y > 0);
    assert(region.z > 0);
    assert(region.w > 0);
    assert(region.x + region.z < d_texture.Width());
    assert(region.y + region.w < d_texture.Height());
    assert(data.size() == region.z * region.w); // width * height

    d_texture.SetSubTexture(region, data);
}

int FontAtlas::Fit(
    std::size_t index,
    std::size_t width,
    std::size_t height)
{
    auto node = d_nodes[index];
    int x = node.x;
    int y = node.y;
    int width_left = width;

    if ((x + width) > (d_texture.Width() - 1)) {
        return -1;
    }

    std::size_t i = index;
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
        region.x = -1; // Full atlas
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