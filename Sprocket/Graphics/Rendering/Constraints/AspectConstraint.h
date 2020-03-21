#pragma once
#include "Window.h"
#include "Quad.h"
#include "Image.h"

namespace Sprocket {

class AspectConstraint
{
public:
    enum class Base
    {
        WIDTH,
        HEIGHT
    };

private:
    Base  d_base;
    float d_aspectRatio;

public:
    AspectConstraint(Base base, float aspectRatio);

    void apply(Window* window, Quad& quad);
    void apply(Window* window, Image& image);
};

}