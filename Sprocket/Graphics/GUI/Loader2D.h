#pragma once
#include "Graphics/GUI/Model2D.h"
#include "Graphics/GUI/Vertex2D.h"

#include <string>
#include <vector>

namespace Sprocket {

class Loader2D
{
private:
    std::vector<unsigned int> d_vaoList;
    std::vector<unsigned int> d_vboList;
    
    unsigned int createVAO();
    void unbindVAO();
    
    void bindVertex2DBuffer(const Vertex2DBuffer& vertex2DBuffer);

public:
    ~Loader2D();

    Model2D load2DModel(const Vertex2DBuffer& vertex2DBuffer);
};

}