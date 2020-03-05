#pragma once
#include <memory>

namespace Sprocket {

class Model
{
    unsigned int d_vaoId;
    int          d_vertexCount;
    unsigned int d_numAttrs;

public:
    Model(unsigned int vaoId, int vertexCount, unsigned int numAttrs);

    unsigned int vaoId() const { return d_vaoId; }
    int vertexCount() const { return d_vertexCount; }

    void bind() const;
    void unbind() const;
};

using ModelPtr = std::shared_ptr<Model>;

}