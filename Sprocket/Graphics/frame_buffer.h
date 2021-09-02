#pragma once
#include <Sprocket/Graphics/Viewport.h>

#include <cstddef>
#include <memory>
#include <optional>

namespace spkt {

class Texture;

class frame_buffer
{
    std::uint32_t d_fbo;

    std::unique_ptr<Texture> d_colour;
    std::unique_ptr<Texture> d_depth;

    int d_width;
    int d_height;

    Viewport d_viewport;

    frame_buffer(const frame_buffer&) = delete;
    frame_buffer& operator=(const frame_buffer&) = delete;

public:
    frame_buffer(int width, int height);
    ~frame_buffer();

    void bind();
    void unbind();
    
    void resize(int width, int height);

    const Texture& colour_texture() const { return *d_colour; }
    const Texture& depth_texture() const { return *d_depth; }
    
    int width() const { return d_width; }
    int height() const { return d_height; }
    glm::ivec2 size() const { return { d_width, d_height }; }
};

using frame_buffer_ptr = std::unique_ptr<frame_buffer>;

}