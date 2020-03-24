#pragma once
#include "Widget.h"
#include "Slider.h"
#include "Button.h"

#include <type_traits>

namespace Sprocket {

struct ContainerAttributes
{
    Maths::vec3 backgroundColour = {0.0f, 0.0f, 0.0f};   
};

class Container : public Widget
{
    ContainerAttributes d_attributes;

    Maths::vec2 d_placementPtr;
    float       d_spacing;

    void updateImpl(Window* window) override;
    bool handleEventImpl(Window* window, const Event& event) override;

public:
    Container(float width,
              const Maths::vec2& placementPtr,
              float spacing,
              const ContainerAttributes& attrs = {});

    VisualQuad& background() { return d_quads[0]; }
    
    const VisualQuad& background() const { return d_quads[0]; }

    ContainerAttributes& attributes() { return d_attributes; }
    const ContainerAttributes& attributes() const { return d_attributes; }

    template <typename T, typename... Args>
    std::shared_ptr<T> add(Args&&... args)
    {
        static_assert(std::is_base_of<Widget, T>::value);

        auto newWidget = std::make_shared<T>(std::forward<Args>(args)...);
        newWidget->position(d_placementPtr);
        makeChild(newWidget);

        d_placementPtr.y += d_spacing + newWidget->height();
        background().body.height += d_spacing + newWidget->height();
        d_base.height += d_spacing + newWidget->height();
        return newWidget;
    }
};

}