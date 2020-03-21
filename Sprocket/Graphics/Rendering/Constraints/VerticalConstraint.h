#pragma once
#include "Window.h"
#include "Quad.h"
#include "Image.h"

namespace Sprocket {

class VerticalConstraint
{
public:
    enum class Type
    {
        TOP,
        CENTRE,
        BOTTOM
    };

private:
    Type  d_type;

    float d_offset;
        // Offset from the side in pixels. This is ignored for
        // CENTRE constraints. TODO: Can we do better than this?

public:
    VerticalConstraint(Type type, float offset);

    void apply(Window* window, Quad& quad);
    void apply(Window* window, Image& image);
};

}