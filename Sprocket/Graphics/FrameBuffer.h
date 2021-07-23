#pragma once
#include <Sprocket/Graphics/Viewport.h>

#include <cstddef>
#include <memory>
#include <optional>

namespace spkt {

class Texture;

class FrameBuffer
{
    std::uint32_t d_fbo;

    std::unique_ptr<Texture> d_colour;
    std::unique_ptr<Texture> d_depth;

    int d_width;
    int d_height;

    Viewport d_viewport;

    FrameBuffer(const FrameBuffer&) = delete;
    FrameBuffer& operator=(const FrameBuffer&) = delete;

public:
    FrameBuffer(int width, int height);
    ~FrameBuffer();

    void Bind();
    void Unbind();
        // Bind/unbind for writing. Any render calls while this is
        // bound for writing will cause objects to be rendered onto
        // this objects texture.

    void BindTexture() const;
    void UnbindTexture() const;

    // An alias for GetColour. TODO: Make FrameBuffer more general
    Texture* GetTexture() const { return GetColour(); }

    Texture* GetColour() const { return d_colour.get(); }
    Texture* GetDepth() const { return d_depth.get(); }

    void SetScreenSize(int width, int height);
        // Resized the internal textures to match the new screen size.

    int Width() const { return d_width; }
    int Height() const { return d_height; }
    glm::ivec2 Size() const { return { Width(), Height() }; }
};

using frame_buffer_ptr = std::unique_ptr<FrameBuffer>;

}