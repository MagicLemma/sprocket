#pragma once

namespace Sprocket {

class RawModel
{
    unsigned int d_vaoId;
    int          d_vertexCount;

public:
    RawModel(unsigned int vaoId, int vertexCount);

    unsigned int vaoId() const { return d_vaoId; }
    size_t vertexCount() const { return d_vertexCount; }

};

}