#include "shape_renderer.h"

#include <Sprocket/Graphics/render_context.h>

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <cstddef>

namespace spkt {
namespace {

constexpr std::vector<quad_vertex> get_quad_vertices()
{
    return { {{-1, -1}}, {{-1, 1}}, {{1, 1}}, {{1, -1}} };
}

constexpr std::vector<std::uint32_t> get_quad_indices()
{
    return {0, 1, 2, 0, 2, 3};
}

}

void quad_vertex::set_buffer_attributes(std::uint32_t vbo)
{
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glEnableVertexAttribArray(0);
    glVertexAttribDivisor(0, 0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(quad_vertex), (void*)offsetof(quad_vertex, position));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void line_instance::set_buffer_attributes(std::uint32_t vbo)
{
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    for (int i = 1; i != 5; ++i) {
        glEnableVertexAttribArray(i);
        glVertexAttribDivisor(i, 1);
    }
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(line_instance), (void*)offsetof(line_instance, begin));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(line_instance), (void*)offsetof(line_instance, end));
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(line_instance), (void*)offsetof(line_instance, colour));
    glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(line_instance), (void*)offsetof(line_instance, thickness));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

shape_renderer::shape_renderer()
    : d_shader("Resources/Shaders/shape.vert", "Resources/Shaders/shape.frag")
    , d_quad_vertices(get_quad_vertices())
    , d_quad_indices(get_quad_indices())
{
}

void shape_renderer::begin_frame(const float width, const float height)
{
    d_shader.bind();
    d_shader.load("u_width", width);
    d_shader.load("u_height", height);
    d_lines.clear();
}

void shape_renderer::end_frame()
{
    spkt::render_context rc;
    rc.alpha_blending(true);
    rc.depth_testing(false);

    d_line_instances.set_data(d_lines);

    d_shader.bind();
    d_quad_vertices.bind();
    d_quad_indices.bind();
    d_line_instances.bind();

    glDrawElementsInstanced(
        GL_TRIANGLES,
        (int)d_quad_indices.size(),
        GL_UNSIGNED_INT,
        nullptr,
        (int)d_line_instances.size()
    );

    d_shader.unbind();
}

void shape_renderer::draw_line(
    const glm::vec2& begin,
    const glm::vec2& end,
    const glm::vec4& colour,
    const float thickness)
{
    d_lines.emplace_back(begin, end, colour, thickness);
}
    
}