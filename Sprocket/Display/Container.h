#pragma once
#include "Widget.h"
#include "Slider.h"
#include "Button.h"

#include <type_traits>

namespace Sprocket {

class Container : public Widget
{
    Maths::vec2 d_placementPtr;
    float       d_spacing;

    void updateImpl(Window* window) override;
    bool handleEventImpl(Window* window, const Event& event) override;
    void drawImpl(DisplayRenderer* renderer) const override;

public:
    Container(float width,
              const Maths::vec2& placementPtr,
              float spacing);

    template <typename T, typename... Args>
    std::shared_ptr<T> add(Args&&... args)
    {
        static_assert(std::is_base_of<Widget, T>::value);

        auto newWidget = std::make_shared<T>(std::forward<Args>(args)...);
        newWidget->position(d_placementPtr);
        makeChild(newWidget);

        d_placementPtr.y += d_spacing + newWidget->height();
        d_base.height += d_spacing + newWidget->height();
        return newWidget;
    }
};

}