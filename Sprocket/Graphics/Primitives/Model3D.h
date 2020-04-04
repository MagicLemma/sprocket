#pragma once
#include "Maths.h"
#include "Resources.h"

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
    std::shared_ptr<VAO> d_vao;
    std::shared_ptr<VBO> d_vertexBuffer;
    std::shared_ptr<VBO> d_indexBuffer;

    int                  d_vertexCount;

    std::shared_ptr<VBO> loadVertexBuffer(const Vertex3DBuffer& vertices);
    std::shared_ptr<VBO> loadIndexBuffer(const IndexBuffer& indices);

public:
    Model3D(const std::string& objFile);
    Model3D(const Vertex3DBuffer& vertices, const IndexBuffer& indices);
    Model3D(); // Empty model

    int vertexCount() const { return d_vertexCount; }

    void bind() const;
    void unbind() const;
};

}