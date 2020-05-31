#include "Slider.h"
#include "MouseEvent.h"
#include "Maths.h"
#include "Log.h"

namespace Sprocket {
    
Slider::Slider(float width, float height)
    : Widget(width, height)
    , d_bar({{0.0, 0.0}, 1.0f, 1.0f})
    , d_picker({{0.0, 0.0}, 1.0f, 1.0f})
    , d_updating(false)
{
}

void Slider::OnUpdateImpl(DisplayRenderer* renderer)
{
    if (d_updating) {
        if (d_mouse.GetMouseOffset().x != 0) {
            float newHorizPos = ToLocalCoords(d_mouse.GetMousePos()).x - d_picker.width/2;
            auto& offset = d_picker.position;
            Maths::Clamp(newHorizPos, Left(), Right());
            offset.x = newHorizPos;
            d_callback(Value());
        }
    }

    renderer->Draw(ToScreenCoords(d_bar));
    renderer->Draw(ToScreenCoords(d_picker));
}

void Slider::OnEventImpl(Event& event)
{
    if (ContainsPoint(Picker(), ToLocalCoords(d_mouse.GetMousePos()))) {
        if (auto e = event.As<MouseButtonPressedEvent>()) {
            if (!e->IsConsumed() && e->Button() == Mouse::LEFT) {
                d_updating = true;
                e->Consume();
            }
        }       
    } else if (ContainsPoint(Bar(), ToLocalCoords(d_mouse.GetMousePos()))) {
        if (auto e = event.As<MouseButtonPressedEvent>()) {
            if (!e->IsConsumed() && e->Button() == Mouse::LEFT) {
                d_updating = true;
                float newHorizPos = ToLocalCoords(d_mouse.GetMousePos()).x - d_picker.width/2;
                auto& offset = d_picker.position;
                Maths::Clamp(newHorizPos, Left(), Right());
                offset.x = newHorizPos;
                d_callback(Value());
                e->Consume();
            }
        }
    }
    
    if (auto e = event.As<MouseButtonReleasedEvent>()) {
        if (e->Button() == Mouse::LEFT) {
            d_updating = false;
        }
    }
}

float Slider::Value() const
{
    return (d_picker.position.x - Left()) / (Right() - Left());
}

float Slider::Left() const
{
    return d_bar.position.x;
}

float Slider::Right() const
{
    return d_bar.position.x + d_bar.width - d_picker.width;
}

void Slider::SetValue(float val)
{
    Maths::Clamp(val, 0.0f, 1.0f);
    d_picker.position.x = Left() + val * (Right() - Left());
    d_callback(val);
}

}