#pragma once
#include "Maths.h"
#include "Window.h"
#include "Event.h"

namespace Sprocket {

class Camera
{
public:
    virtual ~Camera() {};

    virtual Maths::mat4 View() const = 0;
    virtual Maths::vec3 Position() const = 0;

    virtual void OnUpdate(double timeDelta) {}
    virtual void OnEvent(Event& event) {}
};

}