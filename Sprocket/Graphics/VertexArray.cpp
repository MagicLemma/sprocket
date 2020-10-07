#include "VertexArray.h"

#include <glad/glad.h>

namespace Sprocket {

VertexArray::VertexArray()
    : d_vao(std::make_shared<VAO>())
{
}

void VertexArray::SetModel(std::shared_ptr<Mesh> model)
{
    d_model = model;
    glBindVertexArray(d_vao->Value());
    model->Bind();
    glBindVertexArray(0);
}

void VertexArray::SetInstances(std::shared_ptr<Buffer> instanceData)
{
    d_instances = instanceData;
    glBindVertexArray(d_vao->Value());
    instanceData->Bind();
    glBindVertexArray(0);
}

void VertexArray::Draw() const
{
    if (!d_model) { return; }

    glBindVertexArray(d_vao->Value());

    if (d_instances) {
        glDrawElementsInstanced(GL_TRIANGLES, (int)d_model->VertexCount(), GL_UNSIGNED_INT, nullptr, d_instances->Size());
    }
    else {
        glDrawElements(GL_TRIANGLES, (int)d_model->VertexCount(), GL_UNSIGNED_INT, nullptr);
    }

}

}