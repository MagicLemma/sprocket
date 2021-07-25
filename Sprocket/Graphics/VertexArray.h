#pragma once
#include <Sprocket/Graphics/mesh.h>
#include <Sprocket/Graphics/buffer.h>

#include <cstddef>
#include <memory>

namespace spkt {

class VertexArray
{
    std::uint32_t d_vao;
    std::size_t d_vertex_count;
    std::size_t d_instance_count;

    VertexArray(const VertexArray&) = delete;
    VertexArray& operator=(const VertexArray&) = delete;

public:
    VertexArray();
    ~VertexArray();

    void SetModel(spkt::static_mesh* model);
    void SetModel(spkt::animated_mesh* model);
    void SetInstances(spkt::buffer<InstanceData>* instanceData);

    void Draw() const;
};

}