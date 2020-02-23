#include "Layer.h"
#include "Log.h"

#include <chrono>

namespace Sprocket {

Layer::Layer(Status status, bool cursorVisible)
    : d_status(status)
    , d_cursorEnabled(cursorVisible)
    , d_ticker(0.0f)
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

float Layer::layerTicker() const
{
    return d_ticker;
}

bool Layer::callHandleEvent(const Event& event)
{
    return handleEvent(event);
}

void Layer::callUpdate()
{
    if (d_status == Status::NORMAL) {
        d_ticker += 0.01f;
    }
    update();
}

void Layer::callDraw()
{
    if (d_status != Layer::Status::INACTIVE) {
        draw();
    }
}

}