#include "Mesh.h"

#include <cassert>
#include <glad/glad.h>

namespace Sprocket {

Mesh::Mesh(const VertexBuffer& vertices, const IndexBuffer& indices)
    : d_vertexBuffer(std::make_shared<VBO>())
    , d_indexBuffer(std::make_shared<VBO>())
    , d_vertexCount(indices.size())
    , d_layout(sizeof(Vertex), 0)
{
    glBindBuffer(GL_ARRAY_BUFFER, d_vertexBuffer->Value());
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, d_indexBuffer->Value());
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    d_layout.AddAttribute(DataType::FLOAT, 3);
    d_layout.AddAttribute(DataType::FLOAT, 2);
    d_layout.AddAttribute(DataType::FLOAT, 3);
    d_layout.AddAttribute(DataType::FLOAT, 3);
    d_layout.AddAttribute(DataType::FLOAT, 3);
    assert(d_layout.Validate());
}

Mesh::Mesh()
    : d_layout(sizeof(Vertex), 0)
{
    d_layout.AddAttribute(DataType::FLOAT, 3);
    d_layout.AddAttribute(DataType::FLOAT, 2);
    d_layout.AddAttribute(DataType::FLOAT, 3);
    d_layout.AddAttribute(DataType::FLOAT, 3);
    d_layout.AddAttribute(DataType::FLOAT, 3);
    assert(d_layout.Validate());
}

void Mesh::Bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, d_vertexBuffer->Value());
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, d_indexBuffer->Value());
    d_layout.SetAttributes();
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

bool Mesh::operator==(const Mesh& other) const
{
    // Two models are the same if they point to the same VAO.
    return d_vertexBuffer->Value() == other.d_vertexBuffer->Value() &&
           d_indexBuffer->Value() == other.d_indexBuffer->Value();
}

BufferLayout Mesh::GetLayout() const
{
    return d_layout;
}

}