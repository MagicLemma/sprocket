#pragma once
#include "Shader.h"
#include "VertexArray.h"
#include "Entity.h"
#include "Scene.h"

namespace Sprocket {

class ColliderRenderer
{
    Shader  d_shader;

    std::unique_ptr<VertexArray> d_vao;

public:
    ColliderRenderer();

    void Draw(const Entity& camera, Scene& scene);
    void Draw(const Maths::mat4& proj, const Maths::mat4& view, Scene& scene);

    Shader& GetShader() { return d_shader; }
};

}