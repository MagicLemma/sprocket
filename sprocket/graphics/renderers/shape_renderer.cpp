#include "shape_renderer.h"

#include <sprocket/graphics/render_context.h>
#include <sprocket/Core/log.h>

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
    for (int i = 1; i != 6; ++i) {
        glEnableVertexAttribArray(i);
        glVertexAttribDivisor(i, 1);
    }
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(line_instance), (void*)offsetof(line_instance, begin));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(line_instance), (void*)offsetof(line_instance, end));
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(line_instance), (void*)offsetof(line_instance, begin_colour));
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(line_instance), (void*)offsetof(line_instance, end_colour));
    glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, sizeof(line_instance), (void*)offsetof(line_instance, thickness));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void circle_instance::set_buffer_attributes(std::uint32_t vbo)
{
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    for (int i = 1; i != 7; ++i) {
        glEnableVertexAttribArray(i);
        glVertexAttribDivisor(i, 1);
    }
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(circle_instance), (void*)offsetof(circle_instance, centre));
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(circle_instance), (void*)offsetof(circle_instance, inner_radius));
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(circle_instance), (void*)offsetof(circle_instance, outer_radius));
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(circle_instance), (void*)offsetof(circle_instance, begin_colour));
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(circle_instance), (void*)offsetof(circle_instance, end_colour));
    glVertexAttribPointer(6, 1, GL_FLOAT, GL_FALSE, sizeof(circle_instance), (void*)offsetof(circle_instance, angle));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

shape_renderer::shape_renderer()
    : d_quad_vertices(get_quad_vertices())
    , d_quad_indices(get_quad_indices())
    , d_line_shader("Resources/Shaders/line.vert", "Resources/Shaders/line.frag")
    , d_circle_shader("Resources/Shaders/circle.vert", "Resources/Shaders/circle.frag")
{
}

void shape_renderer::begin_frame(const float width, const float height)
{
    d_line_shader.bind();
    d_line_shader.load("u_width", width);
    d_line_shader.load("u_height", height);
    d_lines.clear();

    d_circle_shader.bind();
    d_circle_shader.load("u_width", width);
    d_circle_shader.load("u_height", height);
    d_circles.clear();
}

void shape_renderer::end_frame()
{
    spkt::render_context rc;
    rc.alpha_blending(true);
    rc.depth_testing(false);

    d_quad_vertices.bind();
    d_quad_indices.bind();

    d_line_instances.set_data(d_lines);
    d_line_shader.bind();
    d_line_instances.bind();
    glDrawElementsInstanced(
        GL_TRIANGLES,
        (int)d_quad_indices.size(),
        GL_UNSIGNED_INT,
        nullptr,
        (int)d_line_instances.size()
    );
    d_line_shader.unbind();
    
    d_circle_instances.set_data(d_circles);
    d_circle_shader.bind();
    d_circle_instances.bind();
    glDrawElementsInstanced(
        GL_TRIANGLES,
        (int)d_quad_indices.size(),
        GL_UNSIGNED_INT,
        nullptr,
        (int)d_circle_instances.size()
    );
    d_circle_shader.unbind();
}

void shape_renderer::draw_line(
    const glm::vec2& begin,
    const glm::vec2& end,
    const glm::vec4& begin_colour,
    const glm::vec4& end_colour,
    const float thickness)
{
    d_lines.emplace_back(begin, end, begin_colour, end_colour, thickness);
}

void shape_renderer::draw_circle_shape(
    const glm::vec2& centre,
    const float      inner_radius,
    const float      outer_radius,
    const glm::vec4& begin_colour,
    const glm::vec4& end_colour,
    const float      angle)
{
    d_circles.emplace_back(centre, inner_radius, outer_radius, begin_colour, end_colour, angle);
}

void shape_renderer::draw_circle(
    const glm::vec2& centre,
    const glm::vec4& colour,
    const float radius)
{
    d_circles.emplace_back(centre, 0.0f, radius, colour, colour, 0.0f);
}

void shape_renderer::draw_annulus(
    const glm::vec2& centre,
    const glm::vec4& colour,
    const float inner_radius,
    const float outer_radius)
{
    d_circles.emplace_back(centre, inner_radius, outer_radius, colour, colour, 0.0f);
}
    
}