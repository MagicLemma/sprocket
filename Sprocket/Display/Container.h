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
    Quad        d_background;
    QuadVisuals d_backgroundVisuals;

    Maths::vec2 d_placementPtr;
    float       d_spacing;

    void updateImpl(Window* window) override;
    bool handleEventImpl(Window* window, const Event& event) override;
    void drawImpl(DisplayRenderer* renderer) const override;

public:
    Container(float width,
              const Maths::vec2& placementPtr,
              float spacing,
              const ContainerAttributes& attrs = {});

    Quad& background() { return d_background; }
    const Quad& background() const { return d_background; }

    QuadVisuals& backgroundVisuals() { return d_backgroundVisuals; }
    const QuadVisuals& backgroundVisuals() const { return d_backgroundVisuals; }

    template <typename T, typename... Args>
    std::shared_ptr<T> add(Args&&... args)
    {
        static_assert(std::is_base_of<Widget, T>::value);

        auto newWidget = std::make_shared<T>(std::forward<Args>(args)...);
        newWidget->position(d_placementPtr);
        makeChild(newWidget);

        d_placementPtr.y += d_spacing + newWidget->height();
        d_background.height += d_spacing + newWidget->height();
        d_base.height += d_spacing + newWidget->height();
        return newWidget;
    }
};

}