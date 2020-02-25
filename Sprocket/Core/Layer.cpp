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

bool Layer::handleEvent(const Event& event)
{
    return handleEventImpl(event);
}

void Layer::update(SceneData* data)
{
    if (d_status == Status::NORMAL) {
        d_ticker += 0.01f;
    }
    updateImpl(data);
}

void Layer::draw()
{
    if (d_status != Layer::Status::INACTIVE) {
        drawImpl();
    }
}

}