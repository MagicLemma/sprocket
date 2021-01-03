#include "VertexArray.h"

#include <glad/glad.h>

namespace Sprocket {

VertexArray::VertexArray()
    : d_vao(0)
    , d_model(nullptr)
    , d_instances(nullptr)
{
    glGenVertexArrays(1, &d_vao);
}

VertexArray::~VertexArray()
{
    glDeleteVertexArrays(1, &d_vao);
}

void VertexArray::SetModel(Mesh* model)
{
    d_model = model;
    if (model != nullptr) {
        glBindVertexArray(d_vao);
        model->Bind();
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
    if (!d_model) { return; }

    glBindVertexArray(d_vao);
    if (d_instances != nullptr) {
        glDrawElementsInstanced(GL_TRIANGLES, (int)d_model->VertexCount(), GL_UNSIGNED_INT, nullptr, d_instances->Size());
    }
    else {
        glDrawElements(GL_TRIANGLES, (int)d_model->VertexCount(), GL_UNSIGNED_INT, nullptr);
    }
    glBindVertexArray(0);
}

}