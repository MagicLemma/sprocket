#include "Container.h"
#include "MouseEvent.h"

namespace Sprocket {

Container::Container(float width,
                     const Maths::vec2& placementPtr,
                     float spacing,
                     const ContainerAttributes& attrs)
    : Widget(width, spacing)
    , d_attributes(attrs)
    , d_placementPtr(placementPtr)
    , d_spacing(spacing)
{
    d_quads.push_back({
        {{0.0, 0.0}, width, spacing},
        attrs.backgroundColour
    });
}

void Container::updateImpl(Window* window)
{
}

bool Container::handleEventImpl(Window* window, const Event& event)
{
    return false;
}

}