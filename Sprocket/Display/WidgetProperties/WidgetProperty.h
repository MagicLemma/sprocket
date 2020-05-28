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
    update(Widget* widget) = 0;

    virtual void
    handleEvent(Widget* widget, Event& event) = 0;
};

}