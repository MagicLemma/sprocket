#include "Image.h"
#include "MouseEvent.h"

namespace Sprocket {

Image::Image(const Texture& texture)
    : Widget((float)texture.width(), (float)texture.height())
    , d_background({
        {0.0, 0.0},
        (float)texture.width(),
        (float)texture.height(),
        texture
    })
{
}

void Image::updateImpl(Window* window)
{
}

bool Image::handleEventImpl(Window* window, const Event& event)
{
    return false;
}

void Image::drawImpl(DisplayRenderer* renderer) const
{
    renderer->draw(toScreenCoords(d_background));
}

}