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

    void OnUpdateImpl(DisplayRenderer* renderer) override {};
    void OnEventImpl(Event& event) override {};

public:
    Container(float width,
              const Maths::vec2& placementPtr,
              float spacing);

    template <typename T, typename... Args>
    std::shared_ptr<T> Add(Args&&... args)
    {
        static_assert(std::is_base_of<Widget, T>::value);

        auto newWidget = std::make_shared<T>(std::forward<Args>(args)...);
        newWidget->Position(d_placementPtr);
        MakeChild(newWidget);

        d_placementPtr.y += d_spacing + newWidget->Height();
        d_base.height += d_spacing + newWidget->Height();
        return newWidget;
    }
};

}