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
    , d_fov(Maths::Radians(fov))
    , d_nearPlane(nearPlane)
    , d_farPlane(farPlane)
    , d_projection(Maths::Perspective(d_aspectRatio,
                                      d_fov,
                                      d_nearPlane,
                                      d_farPlane))
{
}

Maths::mat4 PerspectiveLens::Projection() const
{
    return d_projection;
}

void PerspectiveLens::OnEvent(Event& event)
{
    if (auto e = event.As<WindowResizeEvent>()) {
        d_aspectRatio = e->AspectRatio();
        d_projection = Maths::Perspective(d_aspectRatio,
                                          d_fov,
                                          d_nearPlane,
                                          d_farPlane);
    }
}

void PerspectiveLens::FOV(float newFov)
{
    d_fov = Maths::Radians(newFov);
    d_projection = Maths::Perspective(d_aspectRatio,
                                      d_fov,
                                      d_nearPlane,
                                      d_farPlane);
}
    
}