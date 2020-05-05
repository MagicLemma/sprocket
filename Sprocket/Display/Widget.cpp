#include "Widget.h"
#include "Log.h"
#include "MouseEvent.h"

namespace Sprocket {

Widget::Widget(float width, float height)
    : d_parent(nullptr)
    , d_children()
    , d_active(true)
    , d_base({{0.0, 0.0}, width, height})
    , d_offset({0.0, 0.0})
{
}

Widget::~Widget()
{
}

Maths::vec2 Widget::absolutePosition() const
{
    if (d_parent != nullptr) {
        return d_offset + d_parent->absolutePosition();
    }
    return d_offset;
}

Maths::vec2 Widget::toLocalCoords(const Maths::vec2& screenCoords) const
{
    return screenCoords - absolutePosition();
}

Maths::vec2 Widget::toScreenCoords(const Maths::vec2& localCoords) const
{
    return localCoords + absolutePosition();
}

Quad Widget::toLocalCoords(const Quad& screenQuad) const
{
    Quad localQuad = screenQuad;
    localQuad.position -= absolutePosition();
    return localQuad;
}

Quad Widget::toScreenCoords(const Quad& localQuad) const
{
    Quad screenQuad = localQuad;
    screenQuad.position += absolutePosition();
    return screenQuad;
}

void Widget::makeChild(std::shared_ptr<Widget> child)
{
    if (child->d_parent != nullptr) {
        SPKT_LOG_ERROR("Failed to make parent/child Widget relationship!");
    }
    
    d_children.push_back(child);
    child->d_parent = this;
}

void Widget::update(Window* window)
{
    if (d_active) {
        for (const auto& child : d_children) {
            child->update(window);
        }

        updateImpl(window);

        for (const auto& property : d_properties) {
            property->update(this, window);
        }
    }
}

bool Widget::handleEvent(Window* window, const Event& event)
{
    if (d_active) {
        for (const auto& child : d_children) {
            if (child->handleEvent(window, event)) {
                return true;
            }
        }

        if (handleEventImpl(window, event)) {
            return true;
        }

        for (const auto& property : d_properties) {
            if (property->handleEvent(this, window, event)) {
                return true;
            }
        }

        if (event.isInCategory(EventCategory::MOUSE)
            && event.isInCategory(EventCategory::INPUT)) {
            return containsPoint(toScreenCoords(d_base), window->getMousePos());
        }
    }
    
    return false; 
}

void Widget::draw(DisplayRenderer* renderer)
{
    if (!d_active) {
        return;
    }
    
    renderer->draw(toScreenCoords(d_base));

    drawImpl(renderer);

    for (const auto& child : d_children) {
        child->draw(renderer);
    }
}

bool Widget::active() const
{
    if (d_parent) {
        return d_active && d_parent->active();
    }
    return d_active;
}

}