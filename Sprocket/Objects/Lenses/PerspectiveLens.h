#pragma once
#include "Lens.h"
#include "Maths.h"
#include "Window.h"

namespace Sprocket {
    
class PerspectiveLens : public Lens
// Class for representing the projection matrix for rendering.
{
    float d_aspectRatio;
    float d_fov;
    float d_nearPlane;
    float d_farPlane;

    Maths::mat4 d_projection;

public:
    PerspectiveLens(float aspectRatio,
                    float fov = 70.0f,
                    float nearPlane = 0.1f,
                    float farPlane = 1000.0f);

    Maths::mat4 Projection() const override;
        // Returns a projection matrix.

    void OnEvent(Event& event) override;
        // This function may be overriden if the Lens needs to
        // response to events.

    float FOV() const { return d_fov; }
    void FOV(float newFov);
};

}