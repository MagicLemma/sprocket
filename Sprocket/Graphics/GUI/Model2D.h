#pragma once

namespace Sprocket {

// Model class that corresponds to Vertex2D
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