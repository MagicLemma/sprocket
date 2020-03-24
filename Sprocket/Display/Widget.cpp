#include "Widget.h"
#include "Log.h"
#include "MouseEvent.h"

namespace Sprocket {

Widget::Widget(float width, float height)
    : d_parent(nullptr)
    , d_children()
    , d_base({{0.0, 0.0}, width, height})
    , d_active(true)
    , d_quads()
{
}

Widget::~Widget()
{
}

Maths::vec2 Widget::absolutePosition() const
{
    if (d_parent != nullptr) {
        return d_base.position + d_parent->absolutePosition();
    }
    return d_base.position;
}

VisualQuad Widget::toLocalCoords(const VisualQuad& screenQuad) const
{
    VisualQuad localQuad = screenQuad;
    localQuad.body.position -= absolutePosition();
    return localQuad;
}

VisualQuad Widget::toScreenCoords(const VisualQuad& localQuad) const
{
    VisualQuad screenQuad = localQuad;
    screenQuad.body.position += absolutePosition();
    return screenQuad;
}

Maths::vec2 Widget::toLocalCoords(const Maths::vec2& screenCoords) const
{
    return screenCoords - absolutePosition();
}

Maths::vec2 Widget::toScreenCoords(const Maths::vec2& localCoords) const
{
    return localCoords + absolutePosition();
}

std::vector<VisualQuad> Widget::quads() const
{
    std::vector<VisualQuad> ret;
    for (const auto& quad : d_quads) {
        ret.push_back(toScreenCoords(quad));
    }
    return ret;
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
            return containsPoint(getScreenQuad(), window->getMousePos());
        }
    }
    
    return false; 
}

bool Widget::active() const
{
    if (d_parent) {
        return d_active && d_parent->active();
    }
    return d_active;
}

Quad Widget::getScreenQuad() const
{
    Quad base = d_base;
    base.position = absolutePosition();
    return base;
}

}