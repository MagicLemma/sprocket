#pragma once
#include "Core/Window.h"
#include "Graphics/Model.h"
#include "Utility/Maths.h"

namespace Sprocket {

class Quad
{
    Model       d_model;
    Maths::vec2 d_centrePosition;
    float       d_width;
    float       d_height;

    Window*     d_window;

public:
    Quad(const Model& model,
           const Maths::vec2& centrePosition,
           float width,
           float height,
               Window* window);

    Maths::vec2 centrePosition() const { return d_centrePosition; }
    void centrePosition(const Maths::vec2& newPosition) { d_centrePosition = newPosition; }

    float width() const { return d_width; }
    void width(float newWidth) { d_width = newWidth; }

    float height() const { return d_height; }
    void height(float newHeight) { d_height = newHeight; }
};

}