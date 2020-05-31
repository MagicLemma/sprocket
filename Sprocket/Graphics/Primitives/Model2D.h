#pragma once
#include "Maths.h"
#include "Resources.h"

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
    std::shared_ptr<VAO> d_vao;
    std::shared_ptr<VBO> d_vertexBuffer;

    int                  d_vertexCount;

public:
    Model2D(const Vertex2DBuffer& vertices);

    int VertexCount() const { return d_vertexCount; }

    void Bind() const;
    void Unbind() const;
};

}