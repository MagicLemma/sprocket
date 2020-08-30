#pragma once
#include "Maths.h"
#include "Resources.h"

#include <vector>

namespace Sprocket {

struct Vertex2D
{
    Maths::vec3 position;
    Maths::vec2 texture;
};

using Vertex2DBuffer = std::vector<Vertex2D>;

class Model2D
// TODO: Remove this basically useless class.
{
    std::shared_ptr<VAO> d_vao;
    std::shared_ptr<VBO> d_vertexBuffer;

    int                  d_vertexCount;

public:
    Model2D(const Vertex2DBuffer& vertices);
    Model2D();

    int VertexCount() const { return d_vertexCount; }

    void Bind() const;
    void Unbind() const;
};

}