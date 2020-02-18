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

RawModel Loader::load(const std::vector<Vertex>& positions)
{
    unsigned int vaoId = createVAO();
    unsigned int vboId;
    glGenBuffers(1, &vboId);
    d_vboList.push_back(vboId);

    glBindBuffer(GL_ARRAY_BUFFER, vboId);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(Vertex) * positions.size(),
                 positions.data(),
                 GL_STATIC_DRAW);

    glVertexAttribPointer(Sprocket::Vertex::posAttr,
                          Vertex::posCount,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(Vertex),
                          reinterpret_cast<void*>(offsetof(Vertex, position)));
                          
    glVertexAttribPointer(Sprocket::Vertex::texAttr,
                          Vertex::texCount,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(Vertex),
                          reinterpret_cast<void*>(offsetof(Vertex, texture)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    return RawModel(vaoId, positions.size());
}

void Loader::unbind()
{
    glBindVertexArray(0);
}

}