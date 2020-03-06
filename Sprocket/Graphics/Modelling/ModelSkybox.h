#pragma once

#include <vector>
#include <glad/glad.h>

namespace Sprocket {

struct VertexSkybox
{
    static constexpr int posAttr = 0;

    static constexpr int posCount = 3;

    GLfloat position[posCount];
};

using VertexSkyboxBuffer = std::vector<VertexSkybox>;

class ModelSkybox
{
    unsigned int d_vaoId;
    int          d_vertexCount;

public:
    ModelSkybox(unsigned int vaoId, int vertexCount);
    
    unsigned int vaoId() const { return d_vaoId; }
    int vertexCount() const { return d_vertexCount; }

    void bind() const;
    void unbind() const;
};

}