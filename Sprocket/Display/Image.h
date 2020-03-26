#pragma once
#include "Widget.h"
#include "Quad.h"
#include "Maths.h"

#include <string>

namespace Sprocket {

class Image : public Widget
{
    Quad        d_background;
    QuadVisuals d_backgroundVisuals;

    void updateImpl(Window* window) override;
    bool handleEventImpl(Window* window, const Event& event) override;
    void drawImpl(DisplayRenderer* renderer) const override;

public:
    Image(const Texture& texture);

    Quad& background() { return d_background; }
    const Quad& background() const { return d_background; }

    QuadVisuals& backgroundVisuals() { return d_backgroundVisuals; }
    const QuadVisuals& backgroundVisuals() const { return d_backgroundVisuals; }
};

}