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
    std::shared_ptr<TEX> d_texture;
    std::shared_ptr<TEX> d_depth; // TODO: Switch to an RBO

    int d_width;
    int d_height;

public:
    DepthBuffer(Window* window, int width, int height);

    void Bind() const;
    void Unbind() const;

    // TODO: Implement SetScreenSize

    int Width() const { return d_width; }
    int Height() const { return d_height; }

    // TODO: Wrap in Texture object.
    unsigned int DepthTexId() const { return d_depth->Value(); }
    unsigned int ColourTexId() const { return d_texture->Value(); }
};

}