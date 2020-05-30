#pragma once
#include "Resources.h"
#include "Texture.h"

namespace Sprocket {

class FrameBuffer
{
    std::shared_ptr<FBO> d_fbo;
    std::shared_ptr<TEX> d_texture;
    std::shared_ptr<RBO> d_depthBuffer;

    int d_width;
    int d_height;

public:
    FrameBuffer(int width, int height);

    void bind() const;
    void unbind() const;
        // Bind/unbind for writing. Any render calls while this is
        // bound for writing will cause objects to be rendered onto
        // this objects texture.

    void bindTexture() const;
    void unbindTexture() const;

    void setScreenSize(int width, int height);
        // Resized the internal textures to match the new screen size.

    int width() const { return d_width; }
    int height() const { return d_height; }
};

}