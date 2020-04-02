#include "Model3D.h"
#include "Loader.h"
#include "ObjParser.h"

#include <glad/glad.h>

namespace Sprocket {
namespace {

// Given a vertex buffer and index buffer, load them to VBOs and set vertex attributes
// in the currently bound VAO.
void loadToVBOs(const Vertex3DBuffer& vertices,
                const IndexBuffer& indices)
{
    // Bind a Vertex Buffer
    Loader::createVBO(Loader::VBOType::VERTEX_BUFFER);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex3D) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

    // Set Vertex Attributes in the VAO
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D),
                          (void*)offsetof(Vertex3D, position));
                                           
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex3D),
                          (void*)offsetof(Vertex3D, texture));

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D),
                          (void*)offsetof(Vertex3D, normal));

    // Bind an Index Buffer to the VAO
    Loader::createVBO(Loader::VBOType::INDEX_BUFFER);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);
}

}

Model3D::Model3D(const std::string& objFile)
    : d_vaoId(Loader::createVAO())
    , d_vertexCount(0)
{
    auto [vertices, indices] = parseObjFile(objFile);
    d_vertexCount = indices.size();

    // Bind the data into VBOs and bind the attributes to the VAO generated and bound above.
    loadToVBOs(vertices, indices);

    // Unbind the VAO
    glBindVertexArray(0);
}


Model3D::Model3D(const Vertex3DBuffer& vertices,
                 const IndexBuffer& indices)
    : d_vaoId(Loader::createVAO())
    , d_vertexCount(indices.size())
{
    // Bind the data into VBOs and bind the attributes to the VAO generated and bound above.
    loadToVBOs(vertices, indices);

    // Unbind the VAO
    glBindVertexArray(0);
}

Model3D::Model3D(const Model3D& model)
    : d_vaoId(model.d_vaoId)
    , d_vertexCount(model.d_vertexCount)
{
}

void Model3D::bind() const
{
    glBindVertexArray(d_vaoId);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
}

void Model3D::unbind() const
{
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glBindVertexArray(0);
}

}