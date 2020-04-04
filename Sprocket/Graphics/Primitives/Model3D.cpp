#include "Model3D.h"
#include "ObjParser.h"

#include <glad/glad.h>

namespace Sprocket {

Model3D::Model3D(const std::string& objFile)
    : d_vao(std::make_shared<VAO>())
{
    SPKT_LOG_INFO("Loading model '{}'", objFile);
    auto [vertices, indices] = parseObjFile(objFile);
    d_vertexCount = indices.size();
    
    glBindVertexArray(d_vao->value());
    d_vertexBuffer = loadVertexBuffer(vertices);
    d_indexBuffer = loadIndexBuffer(indices);
    glBindVertexArray(0);
}

Model3D::Model3D(const Vertex3DBuffer& vertices, const IndexBuffer& indices)
    : d_vao(std::make_shared<VAO>())
    , d_vertexCount(indices.size())
{
    glBindVertexArray(d_vao->value());
    d_vertexBuffer = loadVertexBuffer(vertices);
    d_indexBuffer = loadIndexBuffer(indices);
    glBindVertexArray(0);
}

Model3D::Model3D()
    : d_vao(nullptr)
    , d_vertexCount(0)
{
}

void Model3D::bind() const
{
    if (d_vao) {
        glBindVertexArray(d_vao->value());
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
    }
    else {
        SPKT_LOG_WARN("Tried to bind an empty Model3D!");
        glBindVertexArray(0);
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
    }
}

void Model3D::unbind() const
{
    
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
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
                                           
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex3D),
                          (void*)offsetof(Vertex3D, texture));

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D),
                          (void*)offsetof(Vertex3D, normal));

    return vertexBuffer;
}

std::shared_ptr<VBO> Model3D::loadIndexBuffer(const IndexBuffer& indices)
{
    auto indexBuffer = std::make_shared<VBO>();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer->value());
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);

    return indexBuffer;
}

}