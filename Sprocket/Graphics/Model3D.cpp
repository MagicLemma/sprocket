#include "Model3D.h"

#include <glad/glad.h>

namespace Sprocket {

Model3D::Model3D(const Vertex3DBuffer& vertices,
                 const IndexBuffer& indices)
{
    d_vao = std::make_shared<VAO>();
    glBindVertexArray(d_vao->Value());
    d_vertexBuffer = LoadVertexBuffer(vertices);
    d_indexBuffer = LoadIndexBuffer(indices);
    glBindVertexArray(0);

    d_vertexData = std::make_shared<Vertex3DBuffer>(vertices);
    d_indexData = std::make_shared<IndexBuffer>(indices);
}

Model3D::Model3D()
{
}

void Model3D::Bind() const
{
    if (d_vao) {
        glBindVertexArray(d_vao->Value());
    }
    else {
        glBindVertexArray(0);
    }
}

void Model3D::Unbind() const
{
    glBindVertexArray(0);
}

std::shared_ptr<VBO> Model3D::LoadVertexBuffer(const Vertex3DBuffer& vertices)
{
    auto vertexBuffer = std::make_shared<VBO>();
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer->Value());
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex3D) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

    // Set Vertex Attributes in the VAO
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D),
                          (void*)offsetof(Vertex3D, position));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D),
                          (void*)offsetof(Vertex3D, normal));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex3D),
                          (void*)offsetof(Vertex3D, textureCoords));

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
    return d_vao->Value() == other.d_vao->Value();
}

void Model3D::Load() const
{
    // Bind the VBO, set the attrib pointers in the VAO, then unbind
    glBindBuffer(GL_ARRAY_BUFFER, d_vertexBuffer->Value());
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D),
                          (void*)offsetof(Vertex3D, position));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D),
                          (void*)offsetof(Vertex3D, normal));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex3D),
                          (void*)offsetof(Vertex3D, textureCoords));
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Set the index buffer pointer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, d_indexBuffer->Value());
}

}