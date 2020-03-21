#pragma once
#include "Quad.h"
#include "Texture.h"
#include "Maths.h"

namespace Sprocket {

class Image
{
    Texture d_texture;
    Quad    d_quad;

    float d_opacity = 1.0f;

public:
    Image(const std::string& pngFile, const Maths::vec2& topLeft);
    Image(const Texture& texture, const Maths::vec2& topLeft);

    Texture texture() const { return d_texture; }
    Texture& texture() { return d_texture; }

    Quad quad() const { return d_quad; }
    Quad& quad() { return d_quad; }

    float opacity() const { return d_opacity; }
    void opacity(float newOpacity) { d_opacity = newOpacity; }
};

}