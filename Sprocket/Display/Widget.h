#pragma once
#include "Window.h"
#include "Event.h"
#include "Quad.h"
#include "WidgetProperty.h"
#include "DisplayRenderer.h"
#include "MouseProxy.h"

#include <variant>
#include <vector>
#include <memory>

namespace Sprocket {

struct WidgetRenderAttributes
{
    bool greyscale = false;
};

class Widget
{
    Widget* d_parent;
        // Non-owning pointer back to this Widgets parent. If this
        // is nullptr, this Widget is assumed to be the root of the tree.

    std::vector<std::shared_ptr<Widget>> d_children;
        // Owning pointers to child widgets.

    std::vector<std::shared_ptr<WidgetProperty>> d_properties;
        // Properties of this widget. Applied on every update
        // and may receive every event.

    Maths::vec2 AbsolutePosition() const;
        // Recurses up the Widget tree to get the position of this
        // Widget with respect to the screen.

    virtual void OnUpdateImpl(DisplayRenderer* renderer) = 0;
    virtual void OnEventImpl(Event& event) = 0;
        // Recurses down through all children and draws them too.

    bool d_active;
        // Stores whether this widget should be active. If it is inactive,
        // it does not update or receive events. They can only be reactivated
        // by external means. Widgets are also inactive if their parents are
        // inactive.

protected:
    Maths::vec2 d_offset;
        // Offset of the top left of this Widget with respect to its parent.
        // If this widget has no parent, this is relative to the top left of
        // the screen. This is the origin of the Widgets "local coordinates".

    Quad d_base;
        // A Quad that represents the position and size of this Widget, with
        // the position being in local coordinates; (0, 0) is the top left of
        // this widget.

    void MakeChild(std::shared_ptr<Widget> child);
        // Sets the given widget to be a child of the current widget.
        // This is protected so external code cannot make widgets children
        // of other widgets, that can only happen within Widget code.

    MouseProxy d_mouse;

public:
    Widget(float width, float height);

    virtual ~Widget();

    void OnUpdate(DisplayRenderer* renderer);
    void OnEvent(Event& event);

    Quad& Base() { return d_base; }
    const Quad& Base() const { return d_base; }

    Maths::vec2 Position() const { return d_offset; }
        // Returns the offset of this Widget relative to it's parent.
        // This is the offset from the screen if the Widget has no parent.
    
    void Position(const Maths::vec2& newPosition) { d_offset = newPosition; }
        // Set the offset of this Widget with respect to its parent.
    
    const std::vector<std::shared_ptr<Widget>>& Children() const { return d_children; }

    float Width() const { return d_base.width; }
    void Width(float newWidth) { d_base.width = newWidth; }

    float Height() const { return d_base.height; }
    void Height(float newHeight) { d_base.height = newHeight; }

    bool Active() const;
    void Active(bool newState) { d_active = newState; }

    template <typename T, typename... Args>
    std::shared_ptr<T> AddProperty(Args&&... args)
    {
        static_assert(std::is_base_of<WidgetProperty, T>::value);
        auto newProperty = std::make_shared<T>(std::forward<Args>(args)...);
        d_properties.push_back(newProperty);
        return newProperty;
    }

public:
    // Helper Functions
    Maths::vec2 ToLocalCoords(const Maths::vec2& screenCoords) const;
    Maths::vec2 ToScreenCoords(const Maths::vec2& localCoords) const;

    Quad ToLocalCoords(const Quad& screenQuad) const;
    Quad ToScreenCoords(const Quad& localQuad) const;
};

}