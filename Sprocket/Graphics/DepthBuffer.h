#pragma once
#include <Sprocket/Graphics/Viewport.h>

#include <cstddef>
#include <memory>

namespace spkt {

class Texture;

class DepthBuffer
// TODO: Merge with FrameBuffer class, as this is just an FBO.
{
    std::uint32_t d_fbo;

    std::unique_ptr<spkt::Texture> d_depth;

    int d_width;
    int d_height;

    spkt::Viewport d_viewport;

    DepthBuffer(const DepthBuffer&) = delete;
    DepthBuffer& operator=(const DepthBuffer&) = delete;

public:
    DepthBuffer(int width, int height);
    ~DepthBuffer();

    void Bind();
    void Unbind();

    int Width() const { return d_width; }
    int Height() const { return d_height; }

    spkt::Texture* GetShadowMap() const { return d_depth.get(); }
};

}