#pragma once
#include "Window.h"
#include "Event.h"
#include "Quad.h"
#include "WidgetProperty.h"
#include "DisplayRenderer.h"

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

    Maths::vec2 absolutePosition() const;
        // Recurses up the Widget tree to get the position of this
        // Widget with respect to the screen.

    virtual void updateImpl(Window* window) = 0;
    virtual bool handleEventImpl(Window* window, const Event& event) = 0;
    virtual void drawImpl(DisplayRenderer* renderer) const = 0;
        // Recurses down through all children and draws them too.

    bool d_active;
        // Stores whether this widget should be active. If it is inactive,
        // it does not update or receive events. They can only be reactivated
        // by external means. Widgets are also inactive if their parents are
        // inactive.

protected:
    Quad d_base;
        // A Quad that represents the position and size of this Widget
        // with respect to its parent, or the screen if this Widget has no
        // parent.

    void makeChild(std::shared_ptr<Widget> child);
        // Sets the given widget to be a child of the current widget.
        // This is protected so external code cannot make widgets children
        // of other widgets, that can only happen within Widget code.

public:
    Widget(float width, float height);
    virtual ~Widget();

    void update(Window* window);
    bool handleEvent(Window* window, const Event& event);
    void draw(DisplayRenderer* renderer);

    Maths::vec2 position() const { return d_base.position; }
        // Returns the offset of this Widget relative to it's parent.
        // This is the offset from the screen if the Widget has no parent.
    
    void position(const Maths::vec2& newPosition) { d_base.position = newPosition; }
        // Set the offset of this Widget with respect to its parent.
    
    const std::vector<std::shared_ptr<Widget>>& children() const { return d_children; }

    float width() const { return d_base.width; }
    void width(float newWidth) { d_base.width = newWidth; }

    float height() const { return d_base.height; }
    void height(float newHeight) { d_base.height = newHeight; }

    bool active() const;
    void active(bool newState) { d_active = newState; }

    template <typename T, typename... Args>
    std::shared_ptr<T> addProperty(Args&&... args)
    {
        static_assert(std::is_base_of<WidgetProperty, T>::value);
        auto newProperty = std::make_shared<T>(std::forward<Args>(args)...);
        d_properties.push_back(newProperty);
        return newProperty;
    }

    Quad getScreenQuad() const;

public:
    // Helper Functions
    Maths::vec2 toLocalCoords(const Maths::vec2& screenCoords) const;
    Maths::vec2 toScreenCoords(const Maths::vec2& localCoords) const;

    Quad toLocalCoords(const Quad& screenQuad) const;
    Quad toScreenCoords(const Quad& localQuad) const;
};

}