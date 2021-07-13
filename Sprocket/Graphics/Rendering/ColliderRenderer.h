#pragma once
#include "Shader.h"
#include "VertexArray.h"

#include "ecs.h"

namespace spkt {

class ColliderRenderer
{
    Shader  d_shader;

    std::unique_ptr<VertexArray> d_vao;

public:
    ColliderRenderer();

    void Draw(const spkt::registry& registry, spkt::entity camera);
    void Draw(const spkt::registry& registry, const glm::mat4& proj, const glm::mat4& view);

    Shader& GetShader() { return d_shader; }
};

}