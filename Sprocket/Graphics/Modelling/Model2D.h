#pragma once

#include <vector>
#include <glad/glad.h>

namespace Sprocket {

struct Vertex2D
{
    GLfloat position[2];
    GLfloat texture[2];
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