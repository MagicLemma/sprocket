#pragma once
#include "Shader.h"
#include "VertexArray.h"
#include "ECS.h"
#include "Scene.h"

namespace spkt {

class ColliderRenderer
{
    Shader  d_shader;

    std::unique_ptr<VertexArray> d_vao;

public:
    ColliderRenderer();

    void Draw(apx::handle camera, Scene& scene);
    void Draw(const glm::mat4& proj, const glm::mat4& view, Scene& scene);

    Shader& GetShader() { return d_shader; }
};

}