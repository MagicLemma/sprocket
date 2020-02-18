#pragma once
#include "RawModel.h"
#include "Vertex.h"

#include <vector>

namespace Sprocket {

class Loader
{
    std::vector<unsigned int> d_vaoList;
    std::vector<unsigned int> d_vboList;
    
    unsigned int createVAO();

public:
    ~Loader();

    RawModel load(const std::vector<Vertex>& positions);

    void unbind();
};

}