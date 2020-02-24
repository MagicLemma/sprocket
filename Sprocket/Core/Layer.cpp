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

bool Layer::callHandleEvent(const Event& event)
{
    return handleEvent(event);
}

void Layer::callUpdate(SceneData* data)
{
    if (d_status == Status::NORMAL) {
        d_ticker += 0.01f;
    }
    update(data);
}

void Layer::callDraw()
{
    if (d_status != Layer::Status::INACTIVE) {
        draw();
    }
}

}