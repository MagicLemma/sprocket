#pragma once

namespace Sprocket {

class FrameBuffer
{
    unsigned int d_fboId;
    unsigned int d_textureId;
    unsigned int d_depthId;

    int d_width;
    int d_height;
    float d_scale;

public:
    FrameBuffer(unsigned int fboId, unsigned int texture, unsigned int depth,
                int width, int height, float scale);

    void bind() const;
    void unbind() const;
        // Bind/unbind for writing. Any render calls while this is
        // bound for writing will cause objects to be rendered onto
        // this objects texture.

    void bindTexture() const;
    void unbindTexture() const;
        // Bind/unbind the texture (colour buffer) for sampling.

    void setScreenSize(int width, int height);
        // Resized the internal textures to match the new screen size.

    unsigned int fboId() const { return d_fboId; }
    unsigned int texture() const { return d_textureId; }
    unsigned int depth() const { return d_depthId; }

    int width() const { return d_width; }
    int height() const { return d_height; }
    float scale() const { return d_scale; }
};

}