#pragma once
#include "Window.h"
#include "Event.h"
#include "WidgetProperty.h"
#include "MouseProxy.h"

namespace Sprocket {

class Widget;

class Draggable : public WidgetProperty
{
    bool       d_moving;
    MouseProxy d_mouse;

public:
    Draggable();
    
    virtual void OnUpdate(Widget* widget) override;
    virtual void OnEvent(Widget* widget, Event& event) override;
};

}