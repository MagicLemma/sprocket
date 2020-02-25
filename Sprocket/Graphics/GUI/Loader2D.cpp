#include "Graphics/GUI/Loader2D.h"
#include "Utility/Log.h"

#include <cstddef>

namespace Sprocket {

Loader2D::~Loader2D()
{
    SPKT_LOG_INFO("Cleaning up 2D VAOs and VBOs");
    glDeleteVertexArrays(d_vaoList.size(), d_vaoList.data());
    glDeleteBuffers(d_vboList.size(), d_vboList.data());
}

unsigned int Loader2D::createVAO()
{
    unsigned int vaoId;
    glGenVertexArrays(1, &vaoId);
    d_vaoList.push_back(vaoId);
    glBindVertexArray(vaoId);
    return vaoId;
}

void Loader2D::bindVertex2DBuffer(const Vertex2DBuffer& vertex2DBuffer)
{
    unsigned int vboId;
    glGenBuffers(1, &vboId);
    d_vboList.push_back(vboId);

    glBindBuffer(GL_ARRAY_BUFFER, vboId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex2D) * vertex2DBuffer.size(), vertex2DBuffer.data(),
                 GL_STATIC_DRAW);

    glVertexAttribPointer(Vertex2D::posAttr, Vertex2D::posCount, GL_FLOAT, GL_FALSE, sizeof(Vertex2D),
                          reinterpret_cast<void*>(offsetof(Vertex2D, position)));
}

Model2D Loader2D::load2DModel(const Vertex2DBuffer& vertex2DBuffer)
{
    unsigned int vaoId = createVAO();
    bindVertex2DBuffer(vertex2DBuffer);
    unbindVAO();

    return Model2D(vaoId, vertex2DBuffer.size());
}

void Loader2D::unbindVAO()
{
    glBindVertexArray(0);
}

}