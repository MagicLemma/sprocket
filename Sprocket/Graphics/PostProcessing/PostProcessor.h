#pragma once
#include "Effect.h"
#include "Model3D.h"

#include <vector>

namespace Sprocket {

using EffectPipeline = std::vector<std::shared_ptr<Effect>>;

class PostProcessor
{
    int d_width;
    int d_height;
        // Dimensions of the screen.

    EffectPipeline d_effects;
    Model3D     d_quad;

public:
    PostProcessor(int width, int height);

    void AddEffect(std::shared_ptr<Effect> effect);

    void Bind() const;
    void Unbind() const;
        // Binds/unbinds the first effect in the pipeline.
        // An error is thrown if no effects have been added.

    void Draw();

    void SetScreenSize(int width, int height);

    template <typename T, typename... Args>
    void AddEffect(Args&&... args) {
        d_effects.push_back(std::make_shared<T>(d_width, d_height, args...));
    }
};

}