#include "Graphics/Terrains/Terrain.h"
#include "Graphics/Loader.h"
#include "Utility/Log.h"
#include "Maths.h"

#include <cstdlib>

namespace Sprocket {
namespace {

std::shared_ptr<Model3D> generateTerrain(int edge, float distance)
{
    Vertex3DBuffer vertices;
    vertices.reserve(edge * edge);
    IndexBuffer indices;
    for (int i = 0; i < edge; ++i) {
        for (int j = 0; j < edge; ++j) {
            Vertex3D v{
                Maths::vec3{distance * i, 0, distance * j},
                Maths::vec2{(float)i / ((float)edge - 1), (float)j / ((float)edge - 1)},
                Maths::vec3{0.0f, 1.0f, 0.0f}
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
    return Loader::loadModel3D(vertices, indices);
}

}

Terrain::Terrain(TexturePtr texture, const Maths::vec3& postition)
    : d_texture(texture)
    , d_position(postition)
    , d_edge(51)
    , d_distance(1.0f)
    , d_model(generateTerrain(d_edge, d_distance))
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