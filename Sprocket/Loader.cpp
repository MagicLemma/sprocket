#include "Loader.h"
#include "Vertex.h"
#include "Log.h"

#include <vector>
#include <cstddef>
#include <glad/glad.h>

namespace Sprocket {

Loader::~Loader()
{
    SPKT_LOG_INFO("Cleaning up VAOs and VBOs");
    glDeleteVertexArrays(d_vaoList.size(), d_vaoList.data());
    glDeleteBuffers(d_vboList.size(), d_vboList.data());
}

unsigned int Loader::createVAO()
{
    unsigned int vaoId;
    glGenVertexArrays(1, &vaoId);
    d_vaoList.push_back(vaoId);
    glBindVertexArray(vaoId);
    return vaoId;
}

void Loader::bindVertexBuffer(const std::vector<Vertex>& vertexBuffer)
{
    unsigned int vboId;
    glGenBuffers(1, &vboId);
    d_vboList.push_back(vboId);

    glBindBuffer(GL_ARRAY_BUFFER, vboId);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(Vertex) * vertexBuffer.size(),
                 vertexBuffer.data(),
                 GL_STATIC_DRAW);

    glVertexAttribPointer(Sprocket::Vertex::posAttr,
                          Vertex::posCount,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(Vertex),
                          reinterpret_cast<void*>(offsetof(Vertex, position)));
                          
    glVertexAttribPointer(Sprocket::Vertex::colAttr,
                          Vertex::colCount,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(Vertex),
                          reinterpret_cast<void*>(offsetof(Vertex, colour)));
}

void Loader::bindIndexBuffer(const std::vector<unsigned int>& indexBuffer)
{
    unsigned int vboId;
    glGenBuffers(1, &vboId);
    d_vboList.push_back(vboId);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 sizeof(unsigned int) * indexBuffer.size(),
                 indexBuffer.data(),
                 GL_STATIC_DRAW);
}

RawModel Loader::load(const VertexBuffer& vertexBuffer,
                      const IndexBuffer& indexBuffer)
{
    unsigned int vaoId = createVAO();

    bindVertexBuffer(vertexBuffer);
    bindIndexBuffer(indexBuffer);

    unbind();
    return RawModel(vaoId, indexBuffer.size());
}

void Loader::unbind()
{
    glBindVertexArray(0);
}

}