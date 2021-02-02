#pragma once
#include "Types.h"
#include "Texture.h"
#include "Viewport.h"

namespace Sprocket {

class DepthBuffer
// TODO: Merge with FrameBuffer class, as this is just an FBO.
{
    u32 d_fbo;

    std::unique_ptr<Texture> d_depth;

    int d_width;
    int d_height;

    Viewport d_viewport;

    DepthBuffer(const DepthBuffer&) = delete;
    DepthBuffer& operator=(const DepthBuffer&) = delete;

public:
    DepthBuffer(int width, int height);
    ~DepthBuffer();

    void Bind();
    void Unbind();

    int Width() const { return d_width; }
    int Height() const { return d_height; }

    Texture* GetShadowMap() const { return d_depth.get(); }
};

}