#include "VertexArray.h"

#include <glad/glad.h>

namespace Sprocket {

VertexArray::VertexArray()
    : d_vao(std::make_shared<VAO>())
{
}

void VertexArray::SetModel(std::shared_ptr<Model3D> model)
{
    d_model = model;
    glBindVertexArray(d_vao->Value());
    model->Bind();

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D),
                          (void*)offsetof(Vertex3D, position));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D),
                          (void*)offsetof(Vertex3D, normal));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex3D),
                          (void*)offsetof(Vertex3D, textureCoords));
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
}

void VertexArray::SetInstances(std::shared_ptr<InstanceBuffer> instanceData)
{
    d_instances = instanceData;
    glBindVertexArray(d_vao->Value());
    instanceData->Bind();

    glEnableVertexAttribArray(3);
    glVertexAttribDivisor(3, 1);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(InstanceData),
                          (void*)offsetof(InstanceData, position));

    glEnableVertexAttribArray(4);
    glVertexAttribDivisor(4, 1);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceData),
                          (void*)offsetof(InstanceData, orientation));

    glEnableVertexAttribArray(5);
    glVertexAttribDivisor(5, 1);
    glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(InstanceData),
                          (void*)offsetof(InstanceData, scale));

    glEnableVertexAttribArray(6);
    glVertexAttribDivisor(6, 1);
    glVertexAttribPointer(6, 1, GL_FLOAT, GL_FALSE, sizeof(InstanceData),
                          (void*)offsetof(InstanceData, shineDamper));

    glEnableVertexAttribArray(7);
    glVertexAttribDivisor(7, 1);
    glVertexAttribPointer(7, 1, GL_FLOAT, GL_FALSE, sizeof(InstanceData),
                          (void*)offsetof(InstanceData, reflectivity));
    glBindBuffer(GL_ARRAY_BUFFER, 0);

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