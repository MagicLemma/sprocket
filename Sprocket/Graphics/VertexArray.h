#pragma once
#include "Resources.h"
#include "Model3D.h"
#include "Buffer.h"

#include <memory>

namespace Sprocket {

class VertexArray
{
    std::shared_ptr<VAO> d_vao;

    std::shared_ptr<Model3D> d_model;
    std::shared_ptr<Buffer> d_instances;

public:
    VertexArray();

    void SetModel(std::shared_ptr<Model3D> model);
    void SetInstances(std::shared_ptr<Buffer> instanceData);

    void Draw() const;
};

}