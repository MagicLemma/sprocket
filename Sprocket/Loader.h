#pragma once
#include "Models/RawModel.h"
#include "Vertex.h"
#include "Texture.h"

#include <vector>

namespace Sprocket {

class Loader
{
public:
    using VertexBuffer = std::vector<Vertex>;
    using IndexBuffer = std::vector<unsigned int>;

private:
    std::vector<unsigned int> d_vaoList;
    std::vector<unsigned int> d_vboList;
    std::vector<unsigned int> d_texList;
    
    unsigned int createVAO();
    
    void bindVertexBuffer(const VertexBuffer& vertexBuffer);
    void bindIndexBuffer(const IndexBuffer& indexBuffer);
    void bindTexture(const Texture& texture);

public:
    ~Loader();

    RawModel load(const VertexBuffer& vertexBuffer,
                  const IndexBuffer& indexBuffer);

    void unbind();
};

}