#pragma once
#include <Sprocket/Graphics/shader.h>
#include <Sprocket/Graphics/buffer.h>

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
    glm::vec4 colour;
    float     thickness;

    static void set_buffer_attributes(std::uint32_t vbo);
};

class shape_renderer
{
    spkt::shader d_shader;

    // Combine these two into a static mesh? This is just a quad to be drawn onto the whole
    // screen. The actual line drawing will be done in the fragment shader.
    const spkt::vertex_buffer<quad_vertex>  d_quad_vertices;
    const spkt::index_buffer<std::uint32_t> d_quad_indices;

    // Maybe give the vertex_buffer an owning vector for streaming?
    std::vector<line_instance>         d_lines;
    spkt::vertex_buffer<line_instance> d_line_instances;

public:
    shape_renderer();

    void begin_frame(const float width, const float height);
    void end_frame();

    void draw_line(
        const glm::vec2& begin,
        const glm::vec2& end,
        const glm::vec4& colour,
        const float thickness
    );
};

}