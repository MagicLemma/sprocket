#pragma once

namespace Sprocket {

class Model
{
    unsigned int d_vaoId;
    int          d_vertexCount;

public:
    Model(unsigned int vaoId, int vertexCount);

    unsigned int vaoId() const { return d_vaoId; }
    int vertexCount() const { return d_vertexCount; }

    void bind() const;
    void unbind() const;
};

}