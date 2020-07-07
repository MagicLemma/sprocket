#pragma once

#include <stdlib.h>
#include <memory>
#include <vector>

#include "vector.h"
#include "vec234.h"

namespace ftgl {

struct texture_atlas_t
{
    /**
     * Allocated nodes
     */
    vector_t * nodes;

    /**
     *  Width (in pixels) of the underlying texture
     */
    size_t width;

    /**
     * Height (in pixels) of the underlying texture
     */
    size_t height;

    /**
     * Depth (in bytes) of the underlying texture
     */
    size_t depth;

    /**
     * Allocated surface size
     */
    size_t used;

    /**
     * Texture identity (OpenGL)
     */
    unsigned int id;

    /**
     * Atlas data
     */
    std::vector<unsigned char> data;

};



/**
 * Creates a new empty texture atlas.
 *
 * @param   width   width of the atlas
 * @param   height  height of the atlas
 * @param   depth   bit depth of the atlas
 * @return          a new empty texture atlas.
 *
 */
  std::shared_ptr<texture_atlas_t>
  texture_atlas_new( const size_t width,
                     const size_t height,
                     const size_t depth );


/**
 *  Allocate a new region in the atlas.
 *
 *  @param self   a texture atlas structure
 *  @param width  width of the region to allocate
 *  @param height height of the region to allocate
 *  @return       Coordinates of the allocated region
 *
 */
  ivec4
  texture_atlas_get_region( std::shared_ptr<texture_atlas_t> self,
                            const size_t width,
                            const size_t height );


/**
 *  Upload data to the specified atlas region.
 *
 *  @param self   a texture atlas structure
 *  @param x      x coordinate the region
 *  @param y      y coordinate the region
 *  @param width  width of the region
 *  @param height height of the region
 *  @param data   data to be uploaded into the specified region
 *  @param stride stride of the data
 *
 */
  void
  texture_atlas_set_region( std::shared_ptr<texture_atlas_t> self,
                            const size_t x,
                            const size_t y,
                            const size_t width,
                            const size_t height,
                            const unsigned char *data,
                            const size_t stride );

/**
 *  Remove all allocated regions from the atlas.
 *
 *  @param self   a texture atlas structure
 */
  void
  texture_atlas_clear( texture_atlas_t * self );


/** @} */

}

