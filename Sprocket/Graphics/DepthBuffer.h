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
    std::shared_ptr<TEX> d_depth; // TODO: Switch to an RBO

    int d_width;
    int d_height;

public:
    DepthBuffer(Window* window, int width, int height);

    void Bind() const;
    void Unbind() const;

    int Width() const { return d_width; }
    int Height() const { return d_height; }

    Texture GetShadowMap() const { return Texture(d_width, d_height, d_depth); }
};

}