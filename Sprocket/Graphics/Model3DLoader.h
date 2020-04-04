#pragma once
#include "Model3D.h"

#include <future>

namespace Sprocket {

struct ModelSpec
{
    Vertex3DBuffer vertices;
    IndexBuffer indices;
};

class Model3DLoader
{
    std::future<ModelSpec> d_loadedModel;

public:
    void load(const std::string& objFile);
    bool completed();
    Model3D get();
};


}