#pragma once
#include "Window.h"
#include "Resources.h"
#include "Texture.h"

namespace Sprocket {

class DepthBuffer
// TODO: Merge with FrameBuffer class, as this is just an FBO.
{
    Window* d_window; 

    std::shared_ptr<FBO> d_fbo;
    std::shared_ptr<Texture> d_depth;

    int d_width;
    int d_height;

    DepthBuffer(const DepthBuffer&) = delete;
    DepthBuffer& operator=(const DepthBuffer&) = delete;

public:
    DepthBuffer(Window* window, int width, int height);

    void Bind() const;
    void Unbind() const;

    int Width() const { return d_width; }
    int Height() const { return d_height; }

    std::shared_ptr<Texture> GetShadowMap() const { return d_depth; }
};

}