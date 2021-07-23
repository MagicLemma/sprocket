#include "PostProcessor.h"

#include <Sprocket/Graphics/Mesh.h>

#include <glad/glad.h>

#include <string>

namespace spkt {
namespace {

StaticMeshData GetMeshData()
{
    StaticMeshData data;

    Vertex bottomLeft;
    bottomLeft.position = {-1.0f, -1.0f, 0.0f};
    bottomLeft.textureCoords = {0.0f, 0.0f};

    Vertex bottomRight;
    bottomRight.position = {1.0f, -1.0f, 0.0f};
    bottomRight.textureCoords = {1.0f, 0.0f};

    Vertex topRight;
    topRight.position = {1.0f, 1.0f, 0.0f};
    topRight.textureCoords = {1.0f, 1.0f};

    Vertex topLeft;
    topLeft.position = {-1.0f, 1.0f, 0.0f};
    topLeft.textureCoords = {0.0f, 1.0f};

    data.vertices = {bottomLeft, bottomRight, topRight, topLeft};
    data.indices = {0, 1, 2, 0, 2, 3};
    return data;
}

}

post_processor::post_processor(int width, int height)
    : d_width(width)
    , d_height(height)
    , d_quad(std::make_unique<Mesh>(GetMeshData()))
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

    d_last_effect = std::make_unique<Shader>(
        std::string(vertex_shader),
        std::string(fragment_shader)
    );
}

void post_processor::start_frame()
{
    d_front_buffer->Bind();
}

void post_processor::set_screen_size(int width, int height)
{
    d_width = width;
    d_height = height;
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
        effect->LoadFloat("targetWidth", (float)d_width);
        effect->LoadFloat("targetHeight", (float)d_height);

        // Swap buffers, so the front is always the target we render to.
        std::swap(d_front_buffer, d_back_buffer);
        d_back_buffer->BindTexture();
        d_front_buffer->Bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    }

    // Finally, apply the last effect to render to the screen
    d_last_effect->Bind();
    d_last_effect->LoadFloat("targetWidth", (float)d_width);
    d_last_effect->LoadFloat("targetHeight", (float)d_height);
    
    d_front_buffer->BindTexture();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}
    
}