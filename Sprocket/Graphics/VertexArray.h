#pragma once
#include "Resources.h"
#include "Mesh.h"
#include "Buffer.h"

#include <memory>

namespace Sprocket {

class VertexArray
{
    std::shared_ptr<VAO> d_vao;

    Mesh* d_model;
    Buffer* d_instances;

public:
    VertexArray();

    void SetModel(Mesh* model);
    void SetInstances(Buffer* instanceData);

    void Draw() const;
};

}