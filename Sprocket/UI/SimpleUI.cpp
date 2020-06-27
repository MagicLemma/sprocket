#include "SimpleUI.h"
#include "KeyboardProxy.h"
#include "MouseProxy.h"
#include "MouseCodes.h"
#include "Log.h"
#include "Maths.h"
#include "RenderContext.h"

#include <glad/glad.h>

namespace Sprocket {

SimpleUI::SimpleUI(Window* window)
    : d_window(window)
    , d_shader("Resources/Shaders/SimpleUI.vert",
               "Resources/Shaders/SimpleUI.frag")
    , d_quadBufferLayout(sizeof(QuadBufferVertex))
{
    d_keyboard.ConsumeAll(false);

    d_quadBufferLayout.AddAttribute(DataType::FLOAT, 2);
    d_quadBufferLayout.AddAttribute(DataType::FLOAT, 4);
    d_quadBufferLayout.AddAttribute(DataType::FLOAT, 2);
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

    if (d_mouse.IsButtonReleased(Mouse::LEFT)) {
        d_clicked = -1;
    }
}

void SimpleUI::StartFrame()
{
    d_quadBufferVertices.clear();
    d_quadBufferIndices.clear();
}

void SimpleUI::EndFrame()
{
    Sprocket::RenderContext rc;  
    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    Maths::mat4 proj = Maths::Ortho(0, d_window->Width(), d_window->Height(), 0);
    d_shader.Bind();
    d_shader.LoadUniform("u_proj_matrix", proj);
    Texture::White().Bind();
    
    d_quadBuffer.Bind();
    d_quadBuffer.SetVertexData(
        sizeof(QuadBufferVertex) * d_quadBufferVertices.size(),
        d_quadBufferVertices.data()
    );

    d_quadBuffer.SetIndexData(
        sizeof(unsigned int) * d_quadBufferIndices.size(),
        d_quadBufferIndices.data()
    );

    glDrawElements(
        GL_TRIANGLES,
        (int)d_quadBufferIndices.size(),
        GL_UNSIGNED_INT,
        nullptr
    );

    Texture::White().Unbind();
    d_shader.Unbind();
    d_quadBuffer.Unbind();
}

bool SimpleUI::Button(
    int id, const std::string& name,
    float x, float y,
    float width, float height)
{
    auto mouse = d_mouse.GetMousePos();
    auto hovered = d_mouse.InRegion(x, y, width, height);
    auto clicked = hovered && d_mouse.IsButtonClicked(Mouse::LEFT);
    if (clicked) { d_clicked = id; }

    Maths::vec4 colour = d_theme.baseColour;
    if (d_clicked == id) {
        colour = d_theme.clickedColour;
    }
    else if (hovered) {
        colour = d_theme.hoveredColour;
    }

    Quad(x, y, width, height, colour);
    return clicked;
}

void SimpleUI::Slider(int id, const std::string& name,
                      float x, float y, float width, float height,
                      float* value, float min, float max)
{
    auto mouse = d_mouse.GetMousePos();
    auto hovered = d_mouse.InRegion(x, y, width, height);
    auto clicked = hovered && d_mouse.IsButtonClicked(Mouse::LEFT);
    if (clicked) { d_clicked = id; }

    float ratio = (*value - min) / (max - min);
    Quad(x, y, ratio * width, height, d_theme.hoveredColour);
    Quad(x + ratio * width, y, (1 - ratio) * width, height, d_theme.baseColour);

    if (d_clicked == id) {
        Maths::Clamp(mouse.x, x, x + width);
        float r = (mouse.x - x) / width;
        *value = (1 - r) * min + r * max;
    }    
}

void SimpleUI::Quad(float x, float y,
                    float width, float height,
                    const Maths::vec4& colour)
{
    unsigned int index = d_quadBufferVertices.size();
    d_quadBufferVertices.push_back({{x,         y},          colour});
    d_quadBufferVertices.push_back({{x + width, y},          colour});
    d_quadBufferVertices.push_back({{x,         y + height}, colour});
    d_quadBufferVertices.push_back({{x + width, y + height}, colour});

    d_quadBufferIndices.push_back(index + 0);
    d_quadBufferIndices.push_back(index + 1);
    d_quadBufferIndices.push_back(index + 2);
    d_quadBufferIndices.push_back(index + 2);
    d_quadBufferIndices.push_back(index + 1);
    d_quadBufferIndices.push_back(index + 3);
}

}