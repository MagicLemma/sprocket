#include "Model3D.h"

#include <glad/glad.h>

namespace Sprocket {

Model3D::Model3D(const Vertex3DBuffer& vertices,
                 const IndexBuffer& indices)
    : d_vertexBuffer(std::make_shared<VBO>())
    , d_indexBuffer(std::make_shared<VBO>())
    , d_count(indices.size())
{
    glBindBuffer(GL_ARRAY_BUFFER, d_vertexBuffer->Value());
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex3D) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, d_indexBuffer->Value());
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

Model3D::Model3D()
{
}

void Model3D::Bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, d_vertexBuffer->Value());
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, d_indexBuffer->Value());
}

bool Model3D::operator==(const Model3D& other) const
{
    // Two models are the same if they point to the same VAO.
    return d_vertexBuffer->Value() == other.d_vertexBuffer->Value() &&
           d_indexBuffer->Value() == other.d_indexBuffer->Value();
}

}