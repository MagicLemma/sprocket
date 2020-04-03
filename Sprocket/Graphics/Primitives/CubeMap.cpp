#include "CubeMap.h"

#include <glad/glad.h>

namespace Sprocket {
    
CubeMap::CubeMap(unsigned int textureId, int width, int height, int bpp)
    : d_textureId(textureId)
    , d_width(width)
    , d_height(height)
    , d_bpp(bpp)
{
}

void CubeMap::bind() const
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, d_textureId);
}

void CubeMap::unbind() const
{
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

}