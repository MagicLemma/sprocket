#pragma once
#include "Widget.h"
#include "Quad.h"
#include "Maths.h"

#include <string>

namespace Sprocket {

class Image : public Widget
{
    void updateImpl(Window* window) override;
    bool handleEventImpl(Window* window, const Event& event) override;

public:
    Image(const Texture& texture);

    VisualQuad& background() { return d_quads[0]; }
    
    const VisualQuad& background() const { return d_quads[0]; }
};

}