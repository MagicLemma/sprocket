#pragma once

#include <vector>
#include <glad/glad.h>

namespace Sprocket {

struct Vertex3D
{
    static constexpr int posAttr = 0;
    static constexpr int texAttr = 1;
    static constexpr int norAttr = 2;

    static constexpr int posCount = 3;
    static constexpr int texCount = 2;
    static constexpr int norCount = 3;

    GLfloat position[posCount];
    GLfloat texture[texCount];
    GLfloat normal[norCount];
};

using Vertex3DBuffer = std::vector<Vertex3D>;
using IndexBuffer = std::vector<unsigned int>;

class Model3D
{
    unsigned int d_vaoId;
    int          d_vertexCount;

public:
    Model3D(unsigned int vaoId, int vertexCount);

    unsigned int vaoId() const { return d_vaoId; }
    int vertexCount() const { return d_vertexCount; }

    void bind() const;
    void unbind() const;
};

}