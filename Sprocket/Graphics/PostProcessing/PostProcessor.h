#pragma once
#include <Sprocket/Graphics/Mesh.h>
#include <Sprocket/Graphics/PostProcessing/Effect.h>

#include <memory>
#include <vector>

namespace spkt {

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