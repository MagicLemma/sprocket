#pragma once
#include "Window.h"
#include "Event.h"

namespace Sprocket {

class Widget;

class WidgetProperty
{
public:
    virtual ~WidgetProperty() {}
    
    virtual void OnUpdate(Widget* widget) = 0;
    virtual void OnEvent(Widget* widget, Event& event) = 0;
};

}