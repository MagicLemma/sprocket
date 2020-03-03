#include "Graphics/Terrain.h"

namespace Sprocket {

Terrain::Terrain(float gridX, float gridZ, const Model& model, const Texture& texture)
    : d_size(800)
    , d_vertexCount(128)
    , d_x(gridX)
    , d_z(gridZ)
    , d_model(model)
    , d_texture(texture)
{
    
}

}