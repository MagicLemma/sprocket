#pragma once
#include "Model.h"
#include "Vertex.h"
#include "Texture.h"

#include <vector>
#include <string>

namespace Sprocket {

class Loader
{
public:
    using VertexBuffer = std::vector<Vertex>;
    using IndexBuffer = std::vector<unsigned int>;
    using TextureFile = std::string;

private:
    std::vector<unsigned int> d_vaoList;
    std::vector<unsigned int> d_vboList;
    std::vector<unsigned int> d_texList;
    
    unsigned int createVAO();
    void unbindVAO();
    
    void bindVertexBuffer(const VertexBuffer& vertexBuffer);
    void bindIndexBuffer(const IndexBuffer& indexBuffer);

public:
    ~Loader();

    Model load(const VertexBuffer& vertexBuffer,
               const IndexBuffer& indexBuffer);

    Texture loadTexture(const TextureFile& textureFile);
};

}