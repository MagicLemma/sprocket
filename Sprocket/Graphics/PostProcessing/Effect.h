#pragma once
#include "Effect.h"
#include "Shader.h"
#include "FrameBuffer.h"

#include <vector>

namespace Sprocket {

class Effect
{
    FrameBuffer d_frameBuffer;

protected:

    Shader       d_shader;

    virtual void SetUniforms() const;
        // Override this function to set any uniforms in the shader.

public:
    Effect(int width, int height,
           const std::string& vertShader,
           const std::string& fragShader);

    virtual ~Effect() {}

    void BindForWrite() const;
    void UnbindForWrite() const;
        // Bind/unbind for writing. Any render calls while this is
        // bound for writing will cause objects to be rendered onto
        // this objects texture.

    void BindForRead() const;
    void UnbindForRead() const;
        // Bind/unbind for reading. Any render calls while this is
        // bound for reading will receive this objects texture which
        // can be sampled in shaders.

    void Draw(std::shared_ptr<Effect> = nullptr);
        // Draws to the texture of the target PostProcess. If this is
        // a nullptr, it is rendered to the screen.

    void SetScreenSize(int width, int height);
        // Resized the internal textures to match the new screen size.

    int Width() const { return d_frameBuffer.Width(); }
    int Height() const { return d_frameBuffer.Height(); }
};

}