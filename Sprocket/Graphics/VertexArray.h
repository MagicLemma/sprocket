#pragma once
#include "Mesh.h"
#include "Buffer.h"

#include <memory>

namespace Sprocket {

class VertexArray
{
    u32 d_vao;

    Mesh* d_model;
    Buffer* d_instances;

    VertexArray(const VertexArray&) = delete;
    VertexArray& operator=(const VertexArray&) = delete;

public:
    VertexArray();
    ~VertexArray();

    void SetModel(Mesh* model);
    void SetInstances(Buffer* instanceData);

    void Draw() const;
};

}