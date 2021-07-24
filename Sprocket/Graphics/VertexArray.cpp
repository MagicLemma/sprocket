#include "VertexArray.h"

#include <glad/glad.h>

namespace spkt {

VertexArray::VertexArray()
    : d_vao(0)
    , d_vertex_count(0)
    , d_instances(nullptr)
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
    d_vertex_count = model->vertex_count();
    if (model != nullptr) {
        glBindVertexArray(d_vao);
        model->bind();
        glBindVertexArray(0);
    }
}

void VertexArray::SetInstances(Buffer* instanceData)
{
    d_instances = instanceData;
    if (instanceData != nullptr) {
        glBindVertexArray(d_vao);
        instanceData->Bind();
        glBindVertexArray(0);
    }
}

void VertexArray::Draw() const
{
    if (d_vertex_count == 0) { return; }

    glBindVertexArray(d_vao);
    if (d_instances != nullptr) {
        glDrawElementsInstanced(GL_TRIANGLES, (int)d_vertex_count, GL_UNSIGNED_INT, nullptr, d_instances->Size());
    }
    else {
        glDrawElements(GL_TRIANGLES, (int)d_vertex_count, GL_UNSIGNED_INT, nullptr);
    }
    glBindVertexArray(0);
}

}