#pragma once
#include "Effect.h"

#include <vector>

namespace Sprocket {

using EffectPipeline = std::vector<std::shared_ptr<Effect>>;

class PostProcessor
{
    int d_width;
    int d_height;
        // Dimensions of the screen.

    EffectPipeline d_effects;

public:
    PostProcessor(int width, int height);

    void addEffect(std::shared_ptr<Effect> effect);

    void bind() const;
    void unbind() const;
        // Binds/unbinds the first effect in the pipeline.
        // An error is thrown if no effects have been added.

    void draw();

    void setScreenSize(int width, int height);

    template <typename T>
    void addEffect() {
        d_effects.push_back(std::make_shared<T>(d_width, d_height));
    }
};

}