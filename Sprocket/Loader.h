#pragma once
#include "RawModel.h"
#include "Vertex.h"

#include <vector>

namespace Sprocket {

class Loader
{
    unsigned int createVAO();

public:

    RawModel load(const std::vector<Vertex>& positions);

    void unbind();
};

}