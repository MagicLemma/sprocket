#include "Model3D.h"
#include "ObjParser.h"

#include <glad/glad.h>

namespace Sprocket {

Model3D::Model3D(const std::string& objFile)
{
    SPKT_LOG_INFO("Loading model '{}'", objFile);
    auto [vertices, indices] = parseObjFile(objFile);

    d_vao = std::make_shared<VAO>();
    glBindVertexArray(d_vao->value());
    d_vertexBuffer = loadVertexBuffer(vertices);
    d_indexBuffer = loadIndexBuffer(indices);
    glBindVertexArray(0);

    d_vertexData = std::make_shared<Vertex3DBuffer>(vertices);
    d_indexData = std::make_shared<IndexBuffer>(indices);
}

Model3D::Model3D(const Vertex3DBuffer& vertices,
                 const IndexBuffer& indices)
{
    d_vao = std::make_shared<VAO>();
    glBindVertexArray(d_vao->value());
    d_vertexBuffer = loadVertexBuffer(vertices);
    d_indexBuffer = loadIndexBuffer(indices);
    glBindVertexArray(0);

    d_vertexData = std::make_shared<Vertex3DBuffer>(vertices);
    d_indexData = std::make_shared<IndexBuffer>(indices);
}

Model3D::Model3D()
{
}

void Model3D::bind() const
{
    if (d_vao) {
        glBindVertexArray(d_vao->value());
    }
    else {
        glBindVertexArray(0);
    }
    
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
}

void Model3D::unbind() const
{
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(3);
    glBindVertexArray(0);
}

std::shared_ptr<VBO> Model3D::loadVertexBuffer(const Vertex3DBuffer& vertices)
{
    auto vertexBuffer = std::make_shared<VBO>();
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer->value());
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex3D) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

    // Set Vertex Attributes in the VAO
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D),
                          (void*)offsetof(Vertex3D, position));

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D),
                          (void*)offsetof(Vertex3D, normal));

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex3D),
                          (void*)offsetof(Vertex3D, textureCoords));

    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex3D),
                          (void*)offsetof(Vertex3D, textureIndex));

    return vertexBuffer;
}

std::shared_ptr<VBO> Model3D::loadIndexBuffer(const IndexBuffer& indices)
{
    auto indexBuffer = std::make_shared<VBO>();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer->value());
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);

    return indexBuffer;
}

bool Model3D::operator==(const Model3D& other) const
{
    // Two models are the same if they point to the same VAO.
    return d_vao->value() == other.d_vao->value();
}

}