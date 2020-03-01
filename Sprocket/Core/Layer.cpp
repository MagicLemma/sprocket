#include "Core/Layer.h"
#include "Utility/Log.h"

#include <chrono>

namespace Sprocket {

Layer::Layer(Status status, bool cursorVisible)
    : d_status(status)
    , d_ticker(0.0f)
{
}

bool Layer::isActive() const
{
    return d_status == Status::NORMAL;
}


float Layer::layerTicker() const
{
    return d_ticker;
}

bool Layer::handleEvent(Window* window, const Event& event)
{
    return handleEventImpl(window, event);
}

void Layer::update(Window* window)
{
    if (d_status == Status::NORMAL) {
        d_ticker += 0.01f;
    }
    updateImpl(window);
}

void Layer::draw(Window* window)
{
    if (d_status != Layer::Status::INACTIVE) {
        drawImpl(window);
    }
}

}