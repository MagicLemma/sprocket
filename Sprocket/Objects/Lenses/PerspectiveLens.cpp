#include "PerspectiveLens.h"
#include "WindowEvent.h"
#include "Log.h"

namespace Sprocket {

PerspectiveLens::PerspectiveLens(float aspectRatio,
                                 float fov,
                                 float nearPlane,
                                 float farPlane)
    : Lens()
    , d_aspectRatio(aspectRatio)
    , d_fov(Maths::radians(fov))
    , d_nearPlane(nearPlane)
    , d_farPlane(farPlane)
    , d_projection(Maths::perspective(d_aspectRatio,
                                      d_fov,
                                      d_nearPlane,
                                      d_farPlane))
{
}

Maths::mat4 PerspectiveLens::projection() const
{
    return d_projection;
}

void PerspectiveLens::handleEvent(Window* window, Event& event)
{
    if (auto e = event.as<WindowResizeEvent>()) {
        d_aspectRatio = e->aspectRatio();
        d_projection = Maths::perspective(d_aspectRatio,
                                          d_fov,
                                          d_nearPlane,
                                          d_farPlane);
    }
}

void PerspectiveLens::fov(float newFov)
{
    d_fov = Maths::radians(newFov);
    d_projection = Maths::perspective(d_aspectRatio,
                                      d_fov,
                                      d_nearPlane,
                                      d_farPlane);
}
    
}