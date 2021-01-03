#pragma once
#include "Window.h"
#include "Types.h"
#include "Resources.h"
#include "Texture.h"

namespace Sprocket {

class DepthBuffer
// TODO: Merge with FrameBuffer class, as this is just an FBO.
{
    Window* d_window; 

    u32 d_fbo;

    std::unique_ptr<Texture> d_depth;

    int d_width;
    int d_height;

    DepthBuffer(const DepthBuffer&) = delete;
    DepthBuffer& operator=(const DepthBuffer&) = delete;

public:
    DepthBuffer(Window* window, int width, int height);
    ~DepthBuffer();

    void Bind() const;
    void Unbind() const;

    int Width() const { return d_width; }
    int Height() const { return d_height; }

    Texture* GetShadowMap() const { return d_depth.get(); }
};

}