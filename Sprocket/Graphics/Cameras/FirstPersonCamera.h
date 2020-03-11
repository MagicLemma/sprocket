#pragma once
#include "Core/Window.h"
#include "Events/Event.h"
#include "Utility/Maths.h"
#include "Camera.h"

namespace Sprocket {

class FirstPersonCamera : public Camera
{
    float d_sensitivity;
        // Mouse sensitivity

    float d_fov;
    float d_nearPlane;
    float d_farPlane;
        // Projection matrix attributes.

    Maths::mat4 d_projectionMatrix;

public:
    FirstPersonCamera(float aspectRatio);

    virtual void update(Window* window, float timeDelta) override;
    void handleEvent(Window* window, const Event& event) override;

    virtual Maths::mat4 projectionMatrix() const override { return d_projectionMatrix; }

    float fov() const { return d_fov; }
    float nearPlane() const { return d_nearPlane; }
    float farPlane() const { return d_farPlane; }
};

}