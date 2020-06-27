#include "SimpleUI.h"
#include "KeyboardProxy.h"
#include "MouseProxy.h"
#include "MouseCodes.h"
#include "Log.h"
#include "Maths.h"

namespace Sprocket {



SimpleUI::SimpleUI(Window* window)
    : d_window(window)
    , d_quadBufferLayout(sizeof(QuadBufferVertex))
{
    d_keyboard.ConsumeAll(false);

    d_quadBufferLayout.AddAttribute(DataType::FLOAT, 2);
    d_quadBufferLayout.AddAttribute(DataType::FLOAT, 3);
    d_quadBuffer.SetBufferLayout(d_quadBufferLayout);
}

void SimpleUI::OnEvent(Event& event)
{
    d_keyboard.OnEvent(event);
    d_mouse.OnEvent(event);
}

void SimpleUI::OnUpdate(double dt)
{
    d_mouse.OnUpdate();
}

void SimpleUI::StartFrame()
{
    d_quadBufferVertices.clear();
    d_quadBufferIndices.clear();
}

void SimpleUI::EndFrame()
{}

bool SimpleUI::Button(
    const std::string& name,
    float x, float y,
    float width, float height)
{

    AddQuad({x, y}, width, height, Maths::vec3{1.0, 0.0, 0.0});

    auto mouse = d_mouse.GetMousePos();
    auto hovered = x < mouse.x && mouse.x < x + width &&
                   y < mouse.y && mouse.y < y + height;

    return hovered && d_mouse.IsButtonClicked(Mouse::LEFT);
}

void SimpleUI::AddQuad(const Maths::vec2& pos,
                       float width, float height,
                       const Maths::vec3& colour)
{
    auto index = d_quadBufferIndices.size();
    d_quadBufferVertices.push_back({{pos.x,         pos.y},          colour});
    d_quadBufferVertices.push_back({{pos.x + width, pos.y},          colour});
    d_quadBufferVertices.push_back({{pos.x,         pos.y + height}, colour});
    d_quadBufferVertices.push_back({{pos.x + width, pos.y + height}, colour});

    d_quadBufferIndices.push_back(index + 0);
    d_quadBufferIndices.push_back(index + 1);
    d_quadBufferIndices.push_back(index + 2);
    d_quadBufferIndices.push_back(index + 2);
    d_quadBufferIndices.push_back(index + 1);
    d_quadBufferIndices.push_back(index + 3);
}

}