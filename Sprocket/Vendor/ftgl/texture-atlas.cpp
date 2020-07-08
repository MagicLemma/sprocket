#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <limits.h>
#include "texture-atlas.h"
#include "Maths.h"

#include <glad/glad.h>

namespace Sprocket {

std::shared_ptr<texture_atlas_t> texture_atlas_new(
    std::size_t width,
    std::size_t height)
{
    auto self = std::make_shared<texture_atlas_t>();
    self->used = 0;
    self->width = width;
    self->height = height;
    self->textureId = 0;

    // We want a one pixel border around the whole atlas to avoid any
    // artefact when sampling texture
    auto node = std::make_shared<Maths::ivec3>();
    node->x = 1;
    node->y = 1;
    node->z = width - 2;

    self->nodes.push_back(node);
    self->data.resize(width*height);
    return self;
}

void texture_atlas_set_region(
    std::shared_ptr<texture_atlas_t> self,
    std::size_t x,
    std::size_t y,
    std::size_t width,
    std::size_t height,
    std::size_t stride,
    const unsigned char* data)
{
    assert( x > 0);
    assert( y > 0);
    assert( x < (self->width-1));
    assert( (x + width) <= (self->width-1));
    assert( y < (self->height-1));
    assert( (y + height) <= (self->height-1));

    // prevent copying data from undefined position
    // and prevent memcpy's undefined behavior when count is zero
    assert(height == 0 || (data != NULL && width > 0));

    glBindTexture(GL_TEXTURE_2D, self->textureId);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexSubImage2D(GL_TEXTURE_2D,
                    0, x, y, width, height, 
                    GL_RED, GL_UNSIGNED_BYTE, (void*)data);
    glBindTexture(GL_TEXTURE_2D, 0);
}

int texture_atlas_fit(
    std::shared_ptr<texture_atlas_t> self,
    std::size_t index,
    std::size_t width,
    std::size_t height)
{
    int x, y, width_left;
    size_t i;

    auto node = self->nodes[index];
    x = node->x;
    y = node->y;
    width_left = width;
    i = index;

    if ( (x + width) > (self->width-1) )
    {
        return -1;
    }
    y = node->y;
    while( width_left > 0 )
    {
        auto node2 = self->nodes[i];
        if( node2->y > y )
        {
            y = node2->y;
        }
        if( (y + height) > (self->height-1) )
        {
            return -1;
        }
        width_left -= node2->z;
        ++i;
    }
    return y;
}

void texture_atlas_merge(std::shared_ptr<texture_atlas_t> self)
{
    for (std::size_t i=0; i< self->nodes.size()-1; ++i )
    {
        auto node = self->nodes[i];
        auto next = self->nodes[i+1];
        if( node->y == next->y )
        {
            node->z += next->z;
            self->nodes.erase(self->nodes.begin() + i + 1);
            --i;
        }
    }
}

Sprocket::Maths::ivec4 texture_atlas_get_region(
    std::shared_ptr<texture_atlas_t> self,
    std::size_t width,
    std::size_t height )
{
    int best_index;
    size_t best_height, best_width;
    Sprocket::Maths::ivec4 region;
    region.x = 0;
    region.y = 0;
    region.z = width;
    region.w = height;

    best_height = UINT_MAX;
    best_index  = -1;
    best_width = UINT_MAX;
    for (std::size_t i = 0; i < self->nodes.size(); ++i)
    {
        int y = texture_atlas_fit(self, i, width, height);
        if (y >= 0)
        {
            auto node = self->nodes[i];
            if( ( (y + height) < best_height ) ||
                ( ((y + height) == best_height) && (node->z > 0 && (size_t)node->z < best_width)) )
            {
                best_height = y + height;
                best_index = i;
                best_width = node->z;
                region.x = node->x;
                region.y = y;
            }
        }
    }

    if( best_index == -1 )
    {
        region.x = -1;
        region.y = -1;
        region.z = 0; // width
        region.w = 0; // height
        return region;
    }

    auto n = std::make_shared<Sprocket::Maths::ivec3>();
    n->x = region.x;
    n->y = region.y + height;
    n->z = width;
    self->nodes.insert(self->nodes.begin() + best_index, n);

    for(std::size_t i = best_index + 1; i < self->nodes.size(); ++i)
    {
        auto node = self->nodes[i];
        auto prev = self->nodes[i-1];

        if (node->x < (prev->x + prev->z) )
        {
            int shrink = prev->x + prev->z - node->x;
            node->x += shrink;
            node->z -= shrink;
            if (node->z <= 0)
            {
                self->nodes.erase(self->nodes.begin() + i);
                --i;
            }
            else
            {
                break;
            }
        }
        else
        {
            break;
        }
    }
    texture_atlas_merge( self );
    self->used += width * height;
    return region;
}

}