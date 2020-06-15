#include "Terrain.h"
#include "Log.h"
#include "Maths.h"

#include <cstdlib>

namespace Sprocket {
namespace {

std::pair<Vertex3DBuffer, IndexBuffer> GenerateTerrain(
    int edge, float distance)
{
    Vertex3DBuffer vertices;
    vertices.reserve(edge * edge);
    IndexBuffer indices;
    for (int i = 0; i < edge; ++i) {
        for (int j = 0; j < edge; ++j) {
            Vertex3D v;
            v.position = Maths::vec3{distance * i, 0, distance * j};
            v.normal = Maths::vec3{0.0f, 1.0f, 0.0f};
            v.textureCoords = Maths::vec2{(float)i / ((float)edge - 1), (float)j / ((float)edge - 1)};
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
    return {vertices, indices};
}

}

Model3D MakeTerrain(int numEdges, float distance)
{
    Vertex3DBuffer vertices;
    vertices.reserve(numEdges * numEdges);
    IndexBuffer indices;
    for (int i = 0; i < numEdges; ++i) {
        for (int j = 0; j < numEdges; ++j) {
            Vertex3D v;
            v.position = Maths::vec3{distance * i, 0, distance * j};
            v.normal = Maths::vec3{0.0f, 1.0f, 0.0f};
            v.textureCoords = Maths::vec2{
                (float)i / ((float)numEdges - 1),
                (float)j / ((float)numEdges - 1)
            };
            vertices.push_back(v);
        }
    }
    for (int i = 0; i != vertices.size() - numEdges; ++i) {
        if ((i + 1) % numEdges != 0) {
            indices.push_back(i);
            indices.push_back(i + 1);
            indices.push_back(i + numEdges);
        }

        if (i % numEdges != 0) {
            indices.push_back(i);
            indices.push_back(i + numEdges);
            indices.push_back(i + numEdges - 1);
        }
        
    }

    return Model3D(vertices, indices);
}

}