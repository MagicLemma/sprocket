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

public:
    ColliderRenderer();

    void Draw(const spkt::registry& registry, spkt::entity camera);
    void Draw(const spkt::registry& registry, const glm::mat4& proj, const glm::mat4& view);

    void draw_box(const glm::mat4& transform, const glm::vec3& half_extents);
    void draw_sphere(const glm::mat4& transform, const float radius);

    shader& GetShader() { return d_shader; }
};

}