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

Maths::vec2 Widget::AbsolutePosition() const
{
    if (d_parent != nullptr) {
        return d_offset + d_parent->AbsolutePosition();
    }
    return d_offset;
}

Maths::vec2 Widget::ToLocalCoords(const Maths::vec2& screenCoords) const
{
    return screenCoords - AbsolutePosition();
}

Maths::vec2 Widget::ToScreenCoords(const Maths::vec2& localCoords) const
{
    return localCoords + AbsolutePosition();
}

Quad Widget::ToLocalCoords(const Quad& screenQuad) const
{
    Quad localQuad = screenQuad;
    localQuad.position -= AbsolutePosition();
    return localQuad;
}

Quad Widget::ToScreenCoords(const Quad& localQuad) const
{
    Quad screenQuad = localQuad;
    screenQuad.position += AbsolutePosition();
    return screenQuad;
}

void Widget::MakeChild(std::shared_ptr<Widget> child)
{
    if (child->d_parent != nullptr) {
        SPKT_LOG_ERROR("Failed to make parent/child Widget relationship!");
    }
    
    d_children.push_back(child);
    child->d_parent = this;
}

void Widget::OnUpdate(DisplayRenderer* renderer)
{
    d_mouse.OnUpdate();

    if (!d_active) {
        return;
    }

    renderer->Draw(ToScreenCoords(d_base));

    for (const auto& property : d_properties) {
        property->OnUpdate(this);
    }

    OnUpdateImpl(renderer);

    for (const auto& child : d_children) {
        child->OnUpdate(renderer);
    }
}

void Widget::OnEvent(Event& event)
{
    d_mouse.OnEvent(event);

    if (d_active) {
        for (const auto& child : d_children) {
            child->OnEvent(event);
        }

        OnEventImpl(event);

        for (const auto& property : d_properties) {
            property->OnEvent(this, event);
        }

        if (event.In<EventCategory::MOUSE>() &&
            event.In<EventCategory::INPUT>() &&
            !event.IsConsumed() &&
            ContainsPoint(ToScreenCoords(d_base), d_mouse.GetMousePos())) {
            
            event.Consume();
        }
    }
}

bool Widget::Active() const
{
    if (d_parent) {
        return d_active && d_parent->Active();
    }
    return d_active;
}

}