#pragma once
#include <Sprocket/Graphics/FrameBuffer.h>
#include <Sprocket/Graphics/Mesh.h>
#include <Sprocket/Graphics/Shader.h>

#include <memory>
#include <vector>

namespace spkt {

class Effect
{
    FrameBuffer d_frameBuffer;
    Shader      d_shader;

public:
    Effect(int width, int height,
           const std::string& vertShader,
           const std::string& fragShader);

    virtual ~Effect() {}

    void BindForWrite();
    void UnbindForWrite();
        // Bind/unbind for writing. Any render calls while this is
        // bound for writing will cause objects to be rendered onto
        // this objects texture.

    void BindForRead() const;
    void UnbindForRead() const;
        // Bind/unbind for reading. Any render calls while this is
        // bound for reading will receive this objects texture which
        // can be sampled in shaders.

    void Draw(Effect* = nullptr);
        // Draws to the texture of the target PostProcess. If this is
        // a nullptr, it is rendered to the screen.

    void SetScreenSize(int width, int height);
        // Resized the internal textures to match the new screen size.
};

std::unique_ptr<Effect> make_gaussian_horiz_effect(int width, int height);
std::unique_ptr<Effect> make_gaussian_vert_effect(int width, int height);
std::unique_ptr<Effect> make_negative_effect(int width, int height);

using EffectPipeline = std::vector<std::unique_ptr<Effect>>;

class PostProcessor
{
    int d_width;
    int d_height;
        // Dimensions of the screen.

    EffectPipeline d_effects;
    Mesh d_quad;

    void AddEffect(std::unique_ptr<Effect> effect);

public:
    PostProcessor(int width, int height);

    void Bind() const;
    void Unbind() const;
        // Binds/unbinds the first effect in the pipeline.
        // An error is thrown if no effects have been added.

    void Draw();

    void SetScreenSize(int width, int height);

    template <typename T, typename... Args>
    void AddEffect(Args&&... args) {
        d_effects.push_back(std::make_unique<T>(d_width, d_height, args...));
    }

    void add_effect(std::unique_ptr<Effect>&& effect)
    {
        d_effects.push_back(std::move(effect));
    }
};

}