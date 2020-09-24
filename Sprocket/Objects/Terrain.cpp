#include "Terrain.h"
#include "Log.h"
#include "Maths.h"

#include <cstdlib>

namespace Sprocket {

Mesh MakeTerrain(int numEdges, float distance)
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

    return Mesh(vertices, indices);
}

}