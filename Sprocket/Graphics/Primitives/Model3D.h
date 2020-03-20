#pragma once
#include "Maths.h"

#include <vector>
#include <string>

namespace Sprocket {

struct Vertex3D
{
    Maths::vec3 position;
    Maths::vec2 texture;
    Maths::vec3 normal;
};

using Vertex3DBuffer = std::vector<Vertex3D>;
using IndexBuffer = std::vector<unsigned int>;

class Model3D
{
    unsigned int d_vaoId;
    int          d_vertexCount;

public:
    Model3D(const std::string& objFile);
    Model3D(const Vertex3DBuffer& vertices, const IndexBuffer& indices);

    unsigned int vaoId() const { return d_vaoId; }
    int vertexCount() const { return d_vertexCount; }

    void bind() const;
    void unbind() const;
};

}