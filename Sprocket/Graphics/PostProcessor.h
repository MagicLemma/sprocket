#pragma once
#include <Sprocket/Graphics/FrameBuffer.h>
#include <Sprocket/Graphics/Mesh.h>
#include <Sprocket/Graphics/Shader.h>

#include <memory>
#include <vector>

namespace spkt {

class post_processor
{
    int d_width;
    int d_height;

    // The quad that is written to the frame buffer when applying the affect,
    // should mostly always match the size of the screen.
    std::unique_ptr<Mesh> d_quad;

    // Post-processing uses two frame buffers, when applying an effect
    // we use one are the source and one as the destination, switching
    // each time.
    std::unique_ptr<FrameBuffer> d_front_buffer;
    std::unique_ptr<FrameBuffer> d_back_buffer;

    std::vector<spkt::shader_ptr> d_effects;
    spkt::shader_ptr d_last_effect;

public:
    post_processor(int width, int height);

    void add_effect(std::string_view vertex_shader, std::string_view fragment_shader);

    // Should be called before rendering the scene. Anything rendered between this
    // and end_frame will be written the post processor rather than the screen.
    void start_frame();

    // Ends the frame, applies all affects and prints to the screen. Anything rendered
    // after this will be rendered to the screen.
    void end_frame();

    void set_screen_size(int width, int height);
};

class Effect
{
    FrameBuffer d_frameBuffer;
    Shader      d_shader;

public:
    Effect(int width, int height,
           const std::string& vertShader,
           const std::string& fragShader);

    virtual ~Effect() {}

    // Bind/unbind for writing. Any render calls while this is
    // bound for writing will cause objects to be rendered onto
    // this objects texture.
    void BindForWrite();
    void UnbindForWrite();

    // Bind/unbind for reading. Any render calls while this is
    // bound for reading will receive this objects texture which
    // can be sampled in shaders.
    void BindForRead() const;
    void UnbindForRead() const;

    // Draws to the texture of the target PostProcess. If this is
    // a nullptr, it is rendered to the screen.
    void Draw(Effect* next = nullptr);

    // Resized the internal textures to match the new screen size.
    void SetScreenSize(int width, int height);
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