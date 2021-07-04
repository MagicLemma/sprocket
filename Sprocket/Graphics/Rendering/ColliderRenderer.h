#pragma once
#include "Shader.h"
#include "VertexArray.h"

#include "apecs.hpp"

namespace spkt {

class ColliderRenderer
{
    Shader  d_shader;

    std::unique_ptr<VertexArray> d_vao;

public:
    ColliderRenderer();

    void Draw(apx::registry& registry, apx::entity camera);
    void Draw(apx::registry& registry, const glm::mat4& proj, const glm::mat4& view);

    Shader& GetShader() { return d_shader; }
};

}