#pragma once

namespace Sprocket {

class Event;

class Layer
{
public:
    virtual void OnEvent(Event& event) = 0;
    virtual void OnUpdate(double dt) = 0;
    virtual void OnRender() = 0;
};

}