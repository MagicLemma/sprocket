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

void VertexArray::Draw() const
{
    glBindVertexArray(d_vao->Value());
    glDrawElements(GL_TRIANGLES, (int)d_model->VertexCount(), GL_UNSIGNED_INT, nullptr);
}

}