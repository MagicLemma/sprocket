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

void Widget::update(Window* window, DisplayRenderer* renderer)
{
    if (!d_active) {
        return;
    }

    renderer->draw(toScreenCoords(d_base));

    for (const auto& property : d_properties) {
        property->update(this, window);
    }

    updateImpl(window, renderer);

    for (const auto& child : d_children) {
        child->update(window, renderer);
    }
}

void Widget::handleEvent(Window* window, Event& event)
{
    if (d_active) {
        for (const auto& child : d_children) {
            child->handleEvent(window, event);
        }

        handleEventImpl(window, event);

        for (const auto& property : d_properties) {
            property->handleEvent(this, window, event);
        }

        if (event.in<EventCategory::MOUSE>() &&
            event.in<EventCategory::INPUT>() &&
            !event.isConsumed() &&
            containsPoint(toScreenCoords(d_base), window->getMousePos())) {
            
            event.consume();
        }
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