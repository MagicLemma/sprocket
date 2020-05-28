#pragma once
#include "Window.h"
#include "Quad.h"
#include "WidgetProperty.h"

namespace Sprocket {

class Widget;

class VerticalConstraint : public WidgetProperty
{
public:
    enum class Type
    {
        TOP,
        CENTRE,
        BOTTOM
    };

private:
    Type  d_type;

    float d_offset;
        // Offset from the side in pixels. This is ignored for
        // CENTRE constraints. TODO: Can we do better than this?

    Window* d_window;

public:
    VerticalConstraint(Type type, float offset, Window* window);

    virtual void update(Widget* widget) override;

    virtual void handleEvent(Widget* widget,
                             Event& event) override {}
};

}