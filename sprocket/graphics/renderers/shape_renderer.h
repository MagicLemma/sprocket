#pragma once
#include <sprocket/graphics/shader.h>
#include <sprocket/graphics/buffer.h>

#include <glm/glm.hpp>

#include <memory>

namespace spkt {

struct quad_vertex
{
    glm::vec2 position;

    static void set_buffer_attributes(std::uint32_t vbo);
};


struct line_instance
{
    glm::vec2 begin;
    glm::vec2 end;
    glm::vec4 begin_colour;
    glm::vec4 end_colour;
    float     thickness;

    static void set_buffer_attributes(std::uint32_t vbo);
};

struct circle_instance
{
    glm::vec2 centre;
    float     inner_radius;
    float     outer_radius;
    glm::vec4 begin_colour;
    glm::vec4 end_colour;
    float     angle;

    static void set_buffer_attributes(std::uint32_t vbo);
};

class shape_renderer
{
    // Combine these two into a static mesh? This is just a quad to be drawn onto the whole
    // screen. The actual line drawing will be done in the fragment shader.
    const spkt::vertex_buffer<quad_vertex>  d_quad_vertices;
    const spkt::index_buffer<std::uint32_t> d_quad_indices;

    spkt::shader                       d_line_shader;
    std::vector<line_instance>         d_lines;
    spkt::vertex_buffer<line_instance> d_line_instances;

    spkt::shader                         d_circle_shader;
    std::vector<circle_instance>         d_circles;
    spkt::vertex_buffer<circle_instance> d_circle_instances;

public:
    shape_renderer();

    void begin_frame(const float width, const float height);
    void end_frame();

    void draw_line(
        const glm::vec2& begin,
        const glm::vec2& end,
        const glm::vec4& begin_colour,
        const glm::vec4& end_colour,
        const float      thickness
    );

    void draw_circle_shape(
        const glm::vec2& centre,
        const float      inner_radius,
        const float      outer_radius,
        const glm::vec4& begin_colour,
        const glm::vec4& end_colour,
        const float      angle
    );

    void draw_circle(
        const glm::vec2& centre,
        const glm::vec4& colour,
        const float      radius
    );

    void draw_annulus(
        const glm::vec2& centre,
        const glm::vec4& colour,
        const float      inner_radius,
        const float      outer_radius
    );
};

}