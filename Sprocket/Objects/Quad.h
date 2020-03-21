#pragma once
#include "Maths.h"

namespace Sprocket {

class Quad
// A class to represent a Quad on the screen. This does not handle
// any vertex buffer data; that will be done by the DisplayRenderer.
{
    Maths::vec2 d_topLeft;
    
    float d_width;
    float d_height;

    Maths::vec3 d_colour;

public:
    Quad(const Maths::vec2& topLeft, float width, float height);

    // Setters
    void topLeft(const Maths::vec2& newTopLeft) { d_topLeft = newTopLeft; }
    void width(float newWidth) { d_width = newWidth; }
    void height(float newHeight) { d_height = newHeight; }

    void xCoord(float x) { d_topLeft.x = x; }
    void yCoord(float y) { d_topLeft.y = y; }

    void scale(float scaleFactor);

    // Getters
    Maths::vec2 topLeft() const { return d_topLeft; }
    Maths::vec3 topLeftV3() const { return {d_topLeft.x, d_topLeft.y, 0.0f}; }

    Maths::vec2 topRight() const { return {right(), top()}; }
    Maths::vec2 bottomLeft() const { return {left(), bottom()}; }
    Maths::vec2 bottomRight() const { return {right(), bottom()}; }

    Maths::vec2 centre() const;

    float width() const { return d_width; }
    float height() const { return d_height; }

    float left() const { return d_topLeft.x; }
    float right() const { return d_topLeft.x + d_width; }
    float top() const { return d_topLeft.y; }
    float bottom() const { return d_topLeft.y + d_height; }

    // Color related
    Maths::vec3 colour() const { return d_colour; }
    void colour(const Maths::vec3& newColour) { d_colour = newColour; }

    // Mouse related
    bool containsPoint(float x, float y) const;
    bool containsPoint(const Maths::vec2& point) const;
};

}