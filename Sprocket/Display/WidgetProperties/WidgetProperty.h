#pragma once
#include "Window.h"
#include "Event.h"

namespace Sprocket {

class Widget;

class WidgetProperty
{
public:
    virtual ~WidgetProperty() {}
    
    virtual void
    update(Widget* widget, Window* window) = 0;

    virtual void
    handleEvent(Widget* widget, Window* window, Event& event) = 0;
};

}