#include "Model3D.h"

#include <glad/glad.h>

namespace Sprocket {

Model3D::Model3D(const Vertex3DBuffer& vertices,
                 const IndexBuffer& indices)
{
    d_vertexBuffer = LoadVertexBuffer(vertices);
    d_indexBuffer = LoadIndexBuffer(indices);

    d_vertexData = std::make_shared<Vertex3DBuffer>(vertices);
    d_indexData = std::make_shared<IndexBuffer>(indices);
}

Model3D::Model3D()
{
}

void Model3D::Bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, d_vertexBuffer->Value());
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, d_indexBuffer->Value());
}

std::shared_ptr<VBO> Model3D::LoadVertexBuffer(const Vertex3DBuffer& vertices)
{
    auto vertexBuffer = std::make_shared<VBO>();
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer->Value());
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex3D) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
    return vertexBuffer;
}

std::shared_ptr<VBO> Model3D::LoadIndexBuffer(const IndexBuffer& indices)
{
    auto indexBuffer = std::make_shared<VBO>();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer->Value());
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);

    return indexBuffer;
}

bool Model3D::operator==(const Model3D& other) const
{
    // Two models are the same if they point to the same VAO.
    return d_vertexBuffer->Value() == other.d_vertexBuffer->Value() &&
           d_indexBuffer->Value() == other.d_indexBuffer->Value();
}

}