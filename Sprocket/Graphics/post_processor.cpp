#include "post_processor.h"

#include <Sprocket/Graphics/mesh.h>
#include <Sprocket/Graphics/Texture.h>

#include <glad/glad.h>

#include <string>
#include <ranges>

namespace spkt {
namespace {

auto ignore_last(int n)
{
    return std::views::reverse | std::views::drop(n) | std::views::reverse;
}

static_mesh_data quad_mesh_data()
{
    return {
        .vertices = {
            { .position = {-1.0f, -1.0f, 0.0f}, .textureCoords = {0.0f, 0.0f} },
            { .position = { 1.0f, -1.0f, 0.0f}, .textureCoords = {1.0f, 0.0f} },
            { .position = { 1.0f,  1.0f, 0.0f}, .textureCoords = {1.0f, 1.0f} },
            { .position = {-1.0f,  1.0f, 0.0f}, .textureCoords = {0.0f, 1.0f} }
        },
        .indices = {0, 1, 2, 0, 2, 3}
    };
}

}

post_processor::post_processor(int width, int height)
    : d_quad(std::make_unique<static_mesh>(quad_mesh_data()))
    , d_target(std::make_unique<frame_buffer>(width, height))
    , d_source(std::make_unique<frame_buffer>(width, height))
    , d_effects()
{}

void post_processor::add_effect(
    std::string_view vertex_shader,
    std::string_view fragment_shader)
{
    d_effects.push_back(std::make_unique<shader>(vertex_shader, fragment_shader));
}

void post_processor::start_frame()
{
    d_target->bind();
}

void post_processor::end_frame()
{
    assert(d_effects.size() > 0);
    d_quad->bind();

    // Apply all effects except for the last.
    for (auto& effect : d_effects | ignore_last(1)) {
        effect->bind();
        effect->load("target_width", d_target->width());
        effect->load("target_height", d_target->height());

        std::swap(d_target, d_source); // Previous render becomes the source
        d_source->colour_texture().bind(0);
        d_target->bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    }

    // Finally, apply the last effect to render to the screen
    d_effects.back()->bind();
    d_effects.back()->load("target_width", d_target->width());
    d_effects.back()->load("target_height", d_target->height());
    
    d_target->colour_texture().bind(0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

void post_processor::set_screen_size(int width, int height)
{
    d_target->resize(width, height);
    d_source->resize(width, height);
}
    
}