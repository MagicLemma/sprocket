#pragma once
#include "Effect.h"
#include "Shader.h"
#include "Model2D.h"

#include <vector>

namespace Sprocket {

class Effect
{
    unsigned int d_fboId;
    unsigned int d_texture;
    unsigned int d_depth;

    Model2D      d_quad;

protected:
    int          d_width;
    int          d_height;
    float        d_scale;

    Shader       d_shader;

    virtual void setUniforms() const;
        // Override this function to set any uniforms in the shader.

public:
    Effect(int width, int height, float scale,
           const std::string& vertShader,
           const std::string& fragShader);

    virtual ~Effect() {}

    void bindForWrite() const;
    void unbindForWrite() const;
        // Bind/unbind for writing. Any render calls while this is
        // bound for writing will cause objects to be rendered onto
        // this objects texture.

    void bindForRead() const;
    void unbindForRead() const;
        // Bind/unbind for reading. Any render calls while this is
        // bound for reading will receive this objects texture which
        // can be sampled in shaders.

    void draw(std::shared_ptr<Effect> = nullptr);
        // Draws to the texture of the target PostProcess. If this is
        // a nullptr, it is rendered to the screen.

    void setScreenSize(int width, int height);
        // Resized the internal textures to match the new screen size.
};

}