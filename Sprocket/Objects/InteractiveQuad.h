#pragma once
#include "Quad.h"
#include "Window.h"
#include "Event.h"
#include "DisplayRenderer.h"

namespace Sprocket {

class InteractiveQuad
{
    Quad d_actual;

    Quad d_normal;
    Quad d_hovered;
    Quad d_clicked;

    bool d_isHovered;
    bool d_isClicked;

public:
    InteractiveQuad(const Quad& normal,
                    const Quad& hovered,
                    const Quad& clicked);

    void update(Window* window);
    bool handleEvent(Window* window, const Event& event);
    void draw(DisplayRenderer* renderer);
};

}