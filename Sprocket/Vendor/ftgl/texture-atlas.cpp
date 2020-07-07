/* Freetype GL - A C OpenGL Freetype engine
 *
 * Distributed under the OSI-approved BSD 2-Clause License.  See accompanying
 * file `LICENSE` for more details.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <limits.h>
#include "texture-atlas.h"

namespace ftgl {


// ------------------------------------------------------ texture_atlas_new ---
std::shared_ptr<texture_atlas_t>
texture_atlas_new( const size_t width,
                   const size_t height,
                   const size_t depth )
{
    auto self = std::make_shared<texture_atlas_t>();

    // We want a one pixel border around the whole atlas to avoid any artefact when
    // sampling texture
    auto node = std::make_shared<ivec3>();
    node->x = 1;
    node->y = 1;
    node->z = width-2;

    assert( (depth == 1) || (depth == 3) || (depth == 4) );
    if( self == NULL)
    {
        fprintf( stderr,
                 "line %d: No more memory for allocating data\n", __LINE__ );
        exit( EXIT_FAILURE );
    }
    self->used = 0;
    self->width = width;
    self->height = height;
    self->depth = depth;
    self->id = 0;

    self->nodes.push_back(node);
    self->data.reserve(width*height*depth);
    return self;
}


// ----------------------------------------------- texture_atlas_set_region ---
void
texture_atlas_set_region(std::shared_ptr<texture_atlas_t> self,
                         const size_t x,
                         const size_t y,
                         const size_t width,
                         const size_t height,
                         const unsigned char * data,
                         const size_t stride )
{
    assert( self );
    assert( x > 0);
    assert( y > 0);
    assert( x < (self->width-1));
    assert( (x + width) <= (self->width-1));
    assert( y < (self->height-1));
    assert( (y + height) <= (self->height-1));

    // prevent copying data from undefined position
    // and prevent memcpy's undefined behavior when count is zero
    assert(height == 0 || (data != NULL && width > 0));

    size_t depth = self->depth;
    size_t charsize = sizeof(char);
    for (std::size_t i=0; i<height; ++i )
    {
        std::memcpy(
            self->data.data()+((y+i)*self->width + x ) * charsize * depth,
            data + (i*stride) * charsize,
            width * charsize * depth
        );
    }
}


// ------------------------------------------------------ texture_atlas_fit ---
int
texture_atlas_fit( std::shared_ptr<texture_atlas_t> self,
                   const size_t index,
                   const size_t width,
                   const size_t height )
{
    int x, y, width_left;
    size_t i;

    assert( self );

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


// ---------------------------------------------------- texture_atlas_merge ---
void
texture_atlas_merge( std::shared_ptr<texture_atlas_t> self )
{
    size_t i;

    assert( self );

    for( i=0; i< self->nodes.size()-1; ++i )
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


// ----------------------------------------------- texture_atlas_get_region ---
ivec4
texture_atlas_get_region( std::shared_ptr<texture_atlas_t> self,
                          const size_t width,
                          const size_t height )
{
    int y, best_index;
    size_t best_height, best_width;
    ivec4 region = {{0,0,width,height}};
    size_t i;

    assert( self );

    best_height = UINT_MAX;
    best_index  = -1;
    best_width = UINT_MAX;
    for( i=0; i<self->nodes.size(); ++i )
    {
        y = texture_atlas_fit( self, i, width, height );
        if( y >= 0 )
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
        region.width = 0;
        region.height = 0;
        return region;
    }

    auto n = std::make_shared<ivec3>();
    n->x = region.x;
    n->y = region.y + height;
    n->z = width;
    self->nodes.insert(self->nodes.begin() + best_index, n);

    for(i = best_index+1; i < self->nodes.size(); ++i)
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