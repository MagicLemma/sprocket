#pragma once
#include <Sprocket/Graphics/Mesh.h>
#include <Sprocket/Graphics/Buffer.h>

#include <cstddef>
#include <memory>

namespace spkt {

class VertexArray
{
    std::uint32_t d_vao;

    spkt::Mesh* d_model;
    spkt::Buffer* d_instances;

    VertexArray(const VertexArray&) = delete;
    VertexArray& operator=(const VertexArray&) = delete;

public:
    VertexArray();
    ~VertexArray();

    void SetModel(spkt::Mesh* model);
    void SetInstances(spkt::Buffer* instanceData);

    void Draw() const;
};

}