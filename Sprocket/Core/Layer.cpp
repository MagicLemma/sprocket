#include "Core/Layer.h"
#include "Utility/Log.h"

namespace Sprocket {

Layer::Layer(const Accessor& accessor, Status status, bool cursorVisible)
    : d_status(status) 
    , d_accessor(accessor)
    , d_ticker(0.0f)
    , d_lastFrame(0.0f)
    , d_deltaTime(0.0f)
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

bool Layer::handleEvent(const Event& event)
{
    return handleEventImpl(event);
}

void Layer::update()
{
    float currentFrame = d_accessor.window()->getTime();
    d_deltaTime = currentFrame - d_lastFrame;
    d_lastFrame = currentFrame;
    if (d_status == Status::NORMAL) {
        d_ticker += d_deltaTime;
    }
    updateImpl();
}

void Layer::draw()
{
    if (d_status != Layer::Status::INACTIVE) {
        drawImpl();
    }
}

void Layer::setAccessor(const Accessor& accessor)
{
    d_accessor = accessor;
}

}