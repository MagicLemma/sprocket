#include "post_processor.h"

#include <Sprocket/Graphics/Mesh.h>

#include <glad/glad.h>

#include <string>
#include <ranges>

namespace spkt {
namespace {

auto ignore_last(int n)
{
    return std::views::reverse | std::views::drop(n) | std::views::reverse;
}

StaticMeshData quad_mesh_data()
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
    : d_quad(std::make_unique<Mesh>(quad_mesh_data()))
    , d_target(std::make_unique<FrameBuffer>(width, height))
    , d_source(std::make_unique<FrameBuffer>(width, height))
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
    d_target->Bind();
}

void post_processor::set_screen_size(int width, int height)
{
    d_target->SetScreenSize(width, height);
    d_source->SetScreenSize(width, height);
}

void post_processor::end_frame()
{
    assert(d_effects.size() > 0);
    d_quad->Bind();

    // Apply all effects except for the last.
    for (auto& effect : d_effects | ignore_last(1)) {
        effect->bind();
        effect->load("target_width", d_target->Width());
        effect->load("target_height", d_target->Height());

        std::swap(d_target, d_source); // Previous render becomes the source
        d_source->BindTexture();
        d_target->Bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    }

    // Finally, apply the last effect to render to the screen
    d_effects.back()->bind();
    d_effects.back()->load("target_width", d_target->Width());
    d_effects.back()->load("target_height", d_target->Height());
    
    d_target->BindTexture();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}
    
}