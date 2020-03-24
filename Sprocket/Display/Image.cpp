#include "Image.h"
#include "MouseEvent.h"

namespace Sprocket {

Image::Image(const Texture& texture)
    : Widget(texture.width(), texture.height())
{
    VisualQuad texturedQuad{
        {{0.0, 0.0}, texture.width(), texture.height()},
        texture
    };

    d_quads.push_back(texturedQuad);
}

void Image::updateImpl(Window* window)
{
}

bool Image::handleEventImpl(Window* window, const Event& event)
{
    return false;
}

}