#include "PostProcessor.h"

#include <Sprocket/Graphics/Mesh.h>

#include <glad/glad.h>

#include <string>

namespace spkt {
namespace {

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
    , d_front_buffer(std::make_unique<FrameBuffer>(width, height))
    , d_back_buffer(std::make_unique<FrameBuffer>(width, height))
    , d_effects()
{}

void post_processor::add_effect(
    std::string_view vertex_shader,
    std::string_view fragment_shader)
{
    if (d_last_effect) {
        d_effects.push_back(std::move(d_last_effect));
    }

    d_last_effect = std::make_unique<Shader>(vertex_shader, fragment_shader);
}

void post_processor::start_frame()
{
    d_front_buffer->Bind();
}

void post_processor::set_screen_size(int width, int height)
{
    d_front_buffer->SetScreenSize(width, height);
    d_back_buffer->SetScreenSize(width, height);
}

void post_processor::end_frame()
{
    d_quad->Bind();

    // Apply all effects except for the last.
    for (auto& effect : d_effects) {
        // Set up the shader. TODO: make uniform uploading more general. 
        effect->Bind();
        effect->LoadInt("target_width", d_front_buffer->Width());
        effect->LoadInt("target_height", d_front_buffer->Height());

        // Swap buffers, so the front is always the target we render to.
        std::swap(d_front_buffer, d_back_buffer);
        d_back_buffer->BindTexture();
        d_front_buffer->Bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    }

    // Finally, apply the last effect to render to the screen
    d_last_effect->Bind();
    d_last_effect->LoadInt("target_width", d_front_buffer->Width());
    d_last_effect->LoadInt("target_height", d_front_buffer->Height());
    
    d_front_buffer->BindTexture();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}
    
}