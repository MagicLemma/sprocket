#pragma once
#include <Sprocket/Graphics/shader.h>
#include <Sprocket/Scene/ecs.h>

#include <glm/glm.hpp>

#include <memory>

namespace spkt {

class VertexArray;

class ColliderRenderer
{
    shader  d_shader;

    std::unique_ptr<VertexArray> d_vao;

public:
    ColliderRenderer();

    void Draw(const spkt::registry& registry, spkt::entity camera);
    void Draw(const spkt::registry& registry, const glm::mat4& proj, const glm::mat4& view);

    shader& GetShader() { return d_shader; }
};

}