#pragma once
#include "Resources.h"
#include "Mesh.h"
#include "Buffer.h"

#include <memory>

namespace Sprocket {

class VertexArray
{
    std::shared_ptr<VAO> d_vao;

    std::shared_ptr<Mesh> d_model;
    std::shared_ptr<Buffer> d_instances;

public:
    VertexArray();

    void SetModel(std::shared_ptr<Mesh> model);
    void SetInstances(std::shared_ptr<Buffer> instanceData);

    void Draw() const;
};

}