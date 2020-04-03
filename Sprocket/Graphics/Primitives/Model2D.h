#pragma once
#include "Maths.h"

#include <vector>

namespace Sprocket {

struct Vertex2D
{
    Maths::vec2 position;
    Maths::vec2 texture;
    Maths::vec3 colour;
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