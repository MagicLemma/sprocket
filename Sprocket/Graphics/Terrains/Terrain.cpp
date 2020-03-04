#include "Graphics/Terrains/Terrain.h"
#include "Graphics/Vertex.h"
#include "Graphics/Loader.h"
#include "Utility/Log.h"

#include <cstdlib>

namespace Sprocket {
namespace {

ModelPtr generateTerrain(int edge, float distance)
{
    VertexBuffer vertices;
    vertices.reserve(edge * edge);
    IndexBuffer indices;
    for (int i = 0; i < edge; ++i) {
        for (int j = 0; j < edge; ++j) {
            Vertex v{
                {distance * i, 0, distance * j},
                {(float)i / ((float)edge - 1), (float)j / ((float)edge - 1)},
                {0.0f, 1.0f, 0.0f}
            };
            vertices.push_back(v);
        }
    }
    for (int i = 0; i != vertices.size() - edge; ++i) {
        if ((i + 1) % edge != 0) {
            indices.push_back(i);
            indices.push_back(i + 1);
            indices.push_back(i + edge);
        }

        if (i % edge != 0) {
            indices.push_back(i);
            indices.push_back(i + edge);
            indices.push_back(i + edge - 1);
        }
        
    }
    return Loader::loadBuffers(vertices, indices);
}

}

Terrain::Terrain(TexturePtr texture)
    : d_model(generateTerrain(100, 0.5f))
    , d_texture(texture)
    , d_position({0.0f, 0.0f, 0.0f})
    , d_edge(10)
    , d_distance(10)
{
}

void Terrain::bind() const
{
    d_model->bind();
    d_texture->bind();
}

void Terrain::unbind() const
{
    d_texture->unbind();
    d_model->unbind();
}

}