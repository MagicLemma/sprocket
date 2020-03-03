#include "Core/Layer.h"
#include "Utility/Log.h"

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


double Layer::layerTicker() const
{
    return d_ticker;
}

bool Layer::handleEvent(Window* window, const Event& event)
{
    return handleEventImpl(window, event);
}

void Layer::update(Window* window)
{
    float currentFrame = window->getTime();
    d_deltaTime = currentFrame - d_lastFrame;
    d_lastFrame = currentFrame;
    if (d_status == Status::NORMAL) {
        d_ticker += d_deltaTime;
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