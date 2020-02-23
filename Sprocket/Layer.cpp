#include "Layer.h"

#include <chrono>

namespace Sprocket {

Layer::Layer(Status status, bool cursorVisible)
    : d_status(status)
    , d_cursorEnabled(cursorVisible)
{
}

bool Layer::isActive() const
{
    return d_status == Status::NORMAL;
}

bool Layer::isCursorVisible() const
{
    return d_cursorEnabled;
}

bool Layer::callHandleEvent(const Event& event)
{
    return handleEvent(event);
}

void Layer::callUpdate(float tick)
{
    update(tick);
}

void Layer::callDraw()
{
    if (d_status != Layer::Status::INACTIVE) {
        draw();
    }
}

}