#include "VertexArray.h"

#include <glad/glad.h>

namespace Sprocket {

VertexArray::VertexArray()
    : d_vao(std::make_shared<VAO>())
    , d_model(nullptr)
    , d_instances(nullptr)
{
}

void VertexArray::SetModel(Mesh* model)
{
    d_model = model;
    if (model != nullptr) {
        glBindVertexArray(d_vao->Value());
        model->Bind();
        glBindVertexArray(0);
    }
}

void VertexArray::SetInstances(Buffer* instanceData)
{
    d_instances = instanceData;
    if (instanceData != nullptr) {
        glBindVertexArray(d_vao->Value());
        instanceData->Bind();
        glBindVertexArray(0);
    }
}

void VertexArray::Draw() const
{
    if (!d_model) { return; }

    glBindVertexArray(d_vao->Value());
    if (d_instances != nullptr) {
        glDrawElementsInstanced(GL_TRIANGLES, (int)d_model->VertexCount(), GL_UNSIGNED_INT, nullptr, d_instances->Size());
    }
    else {
        glDrawElements(GL_TRIANGLES, (int)d_model->VertexCount(), GL_UNSIGNED_INT, nullptr);
    }
    glBindVertexArray(0);
}

}