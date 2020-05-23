#include "Container.h"
#include "MouseEvent.h"

namespace Sprocket {

Container::Container(float width,
                     const Maths::vec2& placementPtr,
                     float spacing)
    : Widget(width, spacing)
    , d_placementPtr(placementPtr)
    , d_spacing(spacing)
{
}

}