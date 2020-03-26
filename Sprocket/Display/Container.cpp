#include "Container.h"
#include "MouseEvent.h"

namespace Sprocket {

Container::Container(float width,
                     const Maths::vec2& placementPtr,
                     float spacing,
                     const ContainerAttributes& attrs)
    : Widget(width, spacing)
    , d_background({
        {0.0, 0.0},
        width,
        spacing
    })
    , d_backgroundVisuals({
        Texture::empty(),
        attrs.backgroundColour
    })
    , d_placementPtr(placementPtr)
    , d_spacing(spacing)
{
}

void Container::updateImpl(Window* window)
{
}

bool Container::handleEventImpl(Window* window, const Event& event)
{
    return false;
}

void Container::drawImpl(DisplayRenderer* renderer) const
{
    renderer->draw(toScreenCoords(d_background), d_backgroundVisuals);
}

}