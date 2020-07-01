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
    , d_bufferLayout(sizeof(QuadBufferVertex))
    , d_font("Resources/Fonts/Calibri.fnt",
             "Resources/Fonts/Calibri.png")
{
    d_keyboard.ConsumeAll(false);

    d_bufferLayout.AddAttribute(DataType::FLOAT, 2);
    d_bufferLayout.AddAttribute(DataType::FLOAT, 4);
    d_bufferLayout.AddAttribute(DataType::FLOAT, 2);
    d_buffer.SetBufferLayout(d_bufferLayout);
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

    d_textBufferVertices.clear();
    d_textBufferIndices.clear();
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
    d_buffer.Bind();
    d_buffer.SetVertexData(
        sizeof(QuadBufferVertex) * d_quadBufferVertices.size(),
        d_quadBufferVertices.data()
    );

    d_buffer.SetIndexData(
        sizeof(unsigned int) * d_quadBufferIndices.size(),
        d_quadBufferIndices.data()
    );

    glDrawElements(
        GL_TRIANGLES,
        (int)d_quadBufferIndices.size(),
        GL_UNSIGNED_INT,
        nullptr
    );

    d_font.Atlas().Bind();
    d_buffer.Bind();
    d_buffer.SetVertexData(
        sizeof(QuadBufferVertex) * d_textBufferVertices.size(),
        d_textBufferVertices.data()
    );

    d_buffer.SetIndexData(
        sizeof(unsigned int) * d_textBufferIndices.size(),
        d_textBufferIndices.data()
    );

    glDrawElements(
        GL_TRIANGLES,
        (int)d_textBufferIndices.size(),
        GL_UNSIGNED_INT,
        nullptr
    );
    d_buffer.Unbind();
    d_font.Atlas().Unbind();
    
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
    
    std::stringstream text;
    text << name << ": " << Maths::ToString(*value, 0);
    
    AddText(x, y, text.str(), 36.0f);

    if (d_clicked == id) {
        Maths::Clamp(mouse.x, x, x + width);
        float r = (mouse.x - x) / width;
        *value = (1 - r) * min + r * max;
    }    
}

void SimpleUI::AddText(float x, float y, const std::string& text, float size)
{
    float fontSize = size / d_font.Size();
    Maths::vec2 pointer(x + 60.0f, y - size); // TODO: Remove this 60.0

    Maths::vec4 colour = {1.0, 1.0, 1.0, 1.0};

    for (int character : text) {
        Character c = d_font.Get(character);

        float xPos = pointer.x + c.XOffset() * fontSize;
        float yPos = pointer.y + (c.Height() - c.YOffset()) * fontSize;

        float width = c.Width() * fontSize;
        float height = c.Height() * fontSize;

        float xTexCoord = c.GetAtlasQuad().position.x;
        float aWidth = c.GetAtlasQuad().width;
        float yTexCoord = c.GetAtlasQuad().position.y;
        float aHeight = c.GetAtlasQuad().height;
        float aw = (float)d_font.Atlas().Width();
        float ah = (float)d_font.Atlas().Height();

        pointer.x += c.Advance() * fontSize;

        unsigned int index = d_textBufferVertices.size();
        d_textBufferVertices.push_back({{xPos,         yPos},          colour, {xTexCoord/aw, yTexCoord/ah}});
        d_textBufferVertices.push_back({{xPos + width, yPos},          colour, {(xTexCoord + aWidth)/aw, yTexCoord/ah}});
        d_textBufferVertices.push_back({{xPos,         yPos + height}, colour, {xTexCoord/aw, (yTexCoord + aHeight)/ah}});
        d_textBufferVertices.push_back({{xPos + width, yPos + height}, colour, {(xTexCoord + aWidth)/aw, (yTexCoord + aHeight)/ah}});

        d_textBufferIndices.push_back(index + 0);
        d_textBufferIndices.push_back(index + 1);
        d_textBufferIndices.push_back(index + 2);
        d_textBufferIndices.push_back(index + 2);
        d_textBufferIndices.push_back(index + 1);
        d_textBufferIndices.push_back(index + 3);
    }
}

}