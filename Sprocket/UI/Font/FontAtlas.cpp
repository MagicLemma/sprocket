#include "FontAtlas.h"

#include <algorithm>
#include <cstdlib>
#include <cassert>
#include <limits>

#include "Maths.h"

namespace Sprocket {

FontAtlas::FontAtlas(int width, int height)
    : d_texture(width, height, Texture::Channels::RED)
{
    // We want a one pixel border around the whole atlas to avoid any
    // artefact when sampling texture
    d_nodes.push_back({1, 1, width - 2});
}

void FontAtlas::SetRegion(
    const Maths::ivec4& region,
    const unsigned char* data)
{
    assert(region.x > 0);
    assert(region.y > 0);
    assert(region.x + region.z < d_texture.Width());
    assert(region.y + region.w < d_texture.Height());
    d_texture.SetSubTexture(region, data);
}

int FontAtlas::Fit(int index, int width, int height)
{
    auto node = d_nodes[index];
    int x = node.x;
    int y = node.y;
    int width_left = width;

    if (x + width >= d_texture.Width()) {
        return -1;
    }

    std::size_t i = index;
    while (width_left > 0) {
        node = d_nodes[i];
        
        y = std::max(y, node.y);
        if (y + height >= d_texture.Height()) {
            return -1;
        }

        width_left -= node.z;
        ++i;
    }
    return y;
}

Maths::ivec4 FontAtlas::GetRegion(std::size_t width, std::size_t height)
{
    // Add padding
    int padding = 1;
    width += 2 * padding;
    height += 2 * padding;

    Maths::ivec4 region{0, 0, width, height};

    std::size_t best_height = std::numeric_limits<std::size_t>::max();
    std::size_t best_width = std::numeric_limits<std::size_t>::max();

    int best_index = -1;

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

    std::size_t i = best_index + 1;
    while (i < d_nodes.size()) {
        auto& node = d_nodes[i];
        auto& prev = d_nodes[i-1];

        if (node.x < prev.x + prev.z) {
            int shrink = prev.x + prev.z - node.x;
            node.x += shrink;
            node.z -= shrink;
            if (node.z <= 0) {
                d_nodes.erase(d_nodes.begin() + i);
            }
        }
        break;
    }
    
    // Merge
    for (std::size_t i = 0; i < d_nodes.size() - 1; ++i) {
        auto& node = d_nodes[i];
        auto& next = d_nodes[i+1];

        if (node.y == next.y) {
            node.z += next.z;
            d_nodes.erase(d_nodes.begin() + i + 1);
            --i;
        }
    }

    // Remove padding
    region.x += padding;
    region.y += padding;
    region.z -= 2 * padding;
    region.w -= 2 * padding;
    return region;
}

}