#pragma once
#include "Resources.h"
#include "Types.h"
#include "Texture.h"

namespace Sprocket {

class FrameBuffer
{
    u32 d_fbo;

    std::shared_ptr<Texture> d_colour;
    std::shared_ptr<Texture> d_depth;

    int d_width;
    int d_height;

    FrameBuffer(const FrameBuffer&) = delete;
    FrameBuffer& operator=(const FrameBuffer&) = delete;

public:
    FrameBuffer(int width, int height);
    ~FrameBuffer();

    void Bind() const;
    void Unbind() const;
        // Bind/unbind for writing. Any render calls while this is
        // bound for writing will cause objects to be rendered onto
        // this objects texture.

    void BindTexture() const;
    void UnbindTexture() const;

    std::shared_ptr<Texture> GetColour() const { return d_colour; }
    std::shared_ptr<Texture> GetDepth() const { return d_depth; }

    void SetScreenSize(int width, int height);
        // Resized the internal textures to match the new screen size.

    int Width() const { return d_width; }
    int Height() const { return d_height; }
};

}