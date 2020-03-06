#pragma once

#include <vector>
#include <glad/glad.h>

namespace Sprocket {

struct Vertex2D
{
    static constexpr int posAttr = 0;
    static constexpr int texAttr = 1;

    static constexpr int posCount = 2;
    static constexpr int texCount = 2;

    GLfloat position[posCount];
    GLfloat texture[texCount];
};

using Vertex2DBuffer = std::vector<Vertex2D>;

class Model2D 
{
    unsigned int d_vaoId;
    int          d_vertexCount;

public:
    Model2D(unsigned int vaoId, int vertexCount);

    unsigned int vaoId() const { return d_vaoId; }
    int vertexCount() const { return d_vertexCount; }

    void bind() const;
    void unbind() const;
};

}