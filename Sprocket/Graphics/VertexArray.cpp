#include "VertexArray.h"

#include <glad/glad.h>

namespace spkt {

VertexArray::VertexArray()
    : d_vao(0)
    , d_vertex_count(0)
    , d_instance_count(0)
{
    glGenVertexArrays(1, &d_vao);
}

VertexArray::~VertexArray()
{
    glDeleteVertexArrays(1, &d_vao);
}

void VertexArray::SetModel(static_mesh* model)
{
    d_vertex_count = model->vertex_count();
    if (model != nullptr) {
        glBindVertexArray(d_vao);
        model->bind();
        glBindVertexArray(0);
    }
}

void VertexArray::SetModel(animated_mesh* model)
{
    assert(model);

    d_vertex_count = model->vertex_count();
    glBindVertexArray(d_vao);
    model->bind();
    glBindVertexArray(0);
}

void VertexArray::SetInstances(spkt::buffer<InstanceData>* instanceData)
{
    if (!instanceData) {
        d_instance_count = 0;
        return;
    }

    d_instance_count = instanceData->size();
    glBindVertexArray(d_vao);
    instanceData->bind();
    glBindVertexArray(0);
}

void VertexArray::Draw() const
{
    if (d_vertex_count == 0) { return; }

    glBindVertexArray(d_vao);
    if (d_instance_count > 0) {
        glDrawElementsInstanced(GL_TRIANGLES, (int)d_vertex_count, GL_UNSIGNED_INT, nullptr, d_instance_count);
    }
    else {
        glDrawElements(GL_TRIANGLES, (int)d_vertex_count, GL_UNSIGNED_INT, nullptr);
    }
    glBindVertexArray(0);
}

}