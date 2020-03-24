#pragma once
#include "Window.h"
#include "Event.h"
#include "WidgetProperty.h"

namespace Sprocket {

class Widget;

class Draggable : public WidgetProperty
{
    bool d_moving;

public:
    Draggable();
    
    virtual void update(Widget* widget, Window* window) override;

    virtual bool handleEvent(Widget* widget,
                             Window* window,
                             const Event& event) override;
};

}