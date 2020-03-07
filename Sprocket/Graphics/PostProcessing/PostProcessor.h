#pragma once
#include "Effect.h"

#include <vector>

namespace Sprocket {

using EffectPipeline = std::vector<std::shared_ptr<Effect>>;

class PostProcessor
{
    EffectPipeline d_effects;

public:
    void addEffect(std::shared_ptr<Effect> effect);

    void bind() const;
    void unbind() const;
        // Binds/unbinds the first effect in the pipeline.
        // An error is thrown if no effects have been added.

    void draw();

    void setScreenSize(int width, int height);
};

}