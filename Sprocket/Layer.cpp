#include "Layer.h"

#include <chrono>

namespace Sprocket {

Layer::Layer(bool isActive, bool cursorVisible, bool isPaused)
    : d_active(isActive)
    , d_cursorEnabled(cursorVisible)
    , d_paused(isPaused)
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