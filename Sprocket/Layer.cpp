#include "Layer.h"

#include <chrono>

namespace Sprocket {

Layer::Layer(bool initialActivation, bool cursorVisible)
    : d_active(initialActivation)
    , d_cursorEnabled(cursorVisible)
{
}

bool Layer::isActive() const
{
    return d_active;
}

bool Layer::isCursorVisible() const
{
    return d_cursorEnabled;
}

}