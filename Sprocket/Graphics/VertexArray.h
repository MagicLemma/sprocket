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
    void SetInstances(spkt::buffer<model_instance>* instanceData);
    void Draw() const;

    void set_model(spkt::static_mesh* model);
    void set_model(spkt::animated_mesh* model);
    void set_instances(spkt::buffer<model_instance>* instanceData);

    void bind() const;
    void unbind() const;

    void draw() const;
};

void draw(spkt::static_mesh* mesh, spkt::buffer<model_instance>* instances = nullptr);
void draw(spkt::animated_mesh* mesh, spkt::buffer<model_instance>* instances = nullptr);

}