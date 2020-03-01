#pragma once
#include "Graphics/Model.h"
#include "Graphics/Vertex.h"
#include "Graphics/Texture.h"

#include <vector>
#include <string>

namespace Sprocket {

class Loader
{
private:
    std::vector<unsigned int> d_vaoList;
    std::vector<unsigned int> d_vboList;
    std::vector<unsigned int> d_texList;
    
    unsigned int createVAO();
    void unbindVAO();
    
    void bindVertexBuffer(const VertexBuffer& vertexBuffer);
    void bindIndexBuffer(const IndexBuffer& indexBuffer);

    void bindVertex2DBuffer(const Vertex2DBuffer& vertex2DBuffer);

public:
    ~Loader();

    Model   loadModel(const std::string& objFile);
    Texture loadTexture(const std::string& textureFile);

    Model   load2DModel(const Vertex2DBuffer& vertex2DBuffer);
};

}