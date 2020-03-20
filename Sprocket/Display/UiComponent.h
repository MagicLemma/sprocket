#pragma once
#include "Window.h"
#include "Event.h"
#include "Quad.h"

namespace Sprocket {

class UiComponent
{
public:
    virtual void update(Window* window) = 0;
    virtual void handleEvent(Window* window, const Event& event) = 0;

    virtual std::vector<Quad> quads() const = 0;
        // Return a vector of all quads for rendering the component.
};

}