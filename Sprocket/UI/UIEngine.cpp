#include "SimpleUI.h"
#include "KeyboardProxy.h"
#include "MouseProxy.h"
#include "MouseCodes.h"
#include "KeyboardCodes.h"
#include "Log.h"
#include "Maths.h"
#include "RenderContext.h"
#include "BufferLayout.h"
#include "Adaptors.h"
#include "KeyboardEvent.h"
#include "MouseEvent.h"

#include <functional>
#include <sstream>
#include <cassert>
#include <algorithm>

#include <glad/glad.h>

namespace Sprocket {
namespace {

bool InRegion(const glm::vec2& pos, const glm::vec4& quad)
{
    float x = quad.x;
    float y = quad.y;
    float width = quad.z;
    float height = quad.w;
    return x < pos.x && pos.x < x + width &&  y < pos.y && pos.y < y + height;
}

std::vector<unsigned char> GetWhiteData()
{
    return {0xff, 0xff, 0xff, 0xff};
}

}

void DrawCommand::AddQuad(const glm::vec4& colour, const glm::vec4& quad)
{
    float x = quad.x;
    float y = quad.y;
    float width = quad.z;
    float height = quad.w;

    std::size_t index = vertices.size();
    vertices.push_back({{x,         y},          colour});
    vertices.push_back({{x + width, y},          colour});
    vertices.push_back({{x,         y + height}, colour});
    vertices.push_back({{x + width, y + height}, colour});

    indices.push_back(index + 0);
    indices.push_back(index + 1);
    indices.push_back(index + 2);
    indices.push_back(index + 2);
    indices.push_back(index + 1);
    indices.push_back(index + 3);
}

void DrawCommand::AddText(const std::string& text,
                          const glm::vec4& quad,
                          const TextProperties& properties)
{
    if (font == nullptr) {
        SPKT_LOG_ERROR("Tried to add text to a draw command with no font!");
        return;
    }

    if (text.size() == 0) {
        return;
    }

    Alignment alignment = properties.alignment;
    float size = properties.size;
    glm::vec4 colour = properties.colour;

    glm::vec2 pen{quad.x, quad.y};

    if (alignment == Alignment::LEFT) {
        pen.x += 5.0f;
        pen.y += size;
    } else if (alignment == Alignment::RIGHT) {
        pen.x += quad.z - 5.0f;
        pen.y += size;
    } else {
        float textWidth = font->TextWidth(text, size);
        Glyph first = font->GetGlyph(text.front(), size);
        pen.y += (quad.w - first.height) / 2.0f;
        pen.x += (quad.z - textWidth) / 2.0f;
        pen.x -= first.offset.x;
        pen.y += first.offset.y;
    }

    for (std::size_t i = 0; i != text.size(); ++i) {
        auto glyph = font->GetGlyph(text[i], size);

        if (i > 0) {
            pen.x += font->GetKerning(text[i-1], text[i], size);
        }

        float xPos = pen.x + glyph.offset.x;
        float yPos = pen.y - glyph.offset.y;

        float width = glyph.width;
        float height = glyph.height;

        float x = glyph.texture.x;
        float y = glyph.texture.y;
        float w = glyph.texture.z;
        float h = glyph.texture.w;

        pen += glyph.advance;

        u32 index = textVertices.size();
        textVertices.push_back({{xPos,         yPos},          colour, {x,     y    }});
        textVertices.push_back({{xPos + width, yPos},          colour, {x + w, y    }});
        textVertices.push_back({{xPos,         yPos + height}, colour, {x,     y + h}});
        textVertices.push_back({{xPos + width, yPos + height}, colour, {x + w, y + h}});

        textIndices.push_back(index + 0);
        textIndices.push_back(index + 1);
        textIndices.push_back(index + 2);
        textIndices.push_back(index + 2);
        textIndices.push_back(index + 1);
        textIndices.push_back(index + 3);
    }
}

UIEngine::UIEngine(Window* window, KeyboardProxy* keyboard, MouseProxy* mouse)
    : d_window(window)
    , d_keyboard(keyboard)
    , d_mouse(mouse)
    , d_shader("Resources/Shaders/SimpleUI.vert",
               "Resources/Shaders/SimpleUI.frag")
    , d_white(1, 1, GetWhiteData().data())
{
    BufferLayout layout(sizeof(BufferVertex));
    layout.AddAttribute(DataType::FLOAT, 2);
    layout.AddAttribute(DataType::FLOAT, 4);
    layout.AddAttribute(DataType::FLOAT, 2);
    d_buffer.SetBufferLayout(layout);
}

glm::vec4 UIEngine::ApplyOffset(const glm::vec4& region)
{
    if (d_currentPanel) {
        glm::vec4 quad = region;
        quad.x += d_currentPanel->region.x;
        quad.y += d_currentPanel->region.y;
        return quad;
    }
    return region;
}

WidgetInfo UIEngine::Register(const std::string& name, const glm::vec4& region)
{
    assert(d_currentPanel);
    
    WidgetInfo info;
    info.quad = ApplyOffset(region);
    
    std::string prefixedName = d_currentPanel->name + "##" + name;
    std::size_t hash = std::hash<std::string>{}(prefixedName);
    d_currentPanel->widgetRegions.push_back({hash, info.quad});

    if (hash == d_clicked) {
        info.sinceClicked = d_time - d_widgetTimes[hash].clickedTime;
        info.sinceUnlicked = 0.0;
    }
    else {
        info.sinceClicked = 0.0;
        info.sinceUnlicked = d_time - d_widgetTimes[hash].unclickedTime;
    }

    if (hash == d_hovered) {
        info.sinceHovered = d_time - d_widgetTimes[hash].hoveredTime;
        info.sinceUnhovered = 0.0;
    }
    else {
        info.sinceHovered = 0.0;
        info.sinceUnhovered = d_time - d_widgetTimes[hash].unhoveredTime;
    }

    if (hash == d_focused) {
        info.sinceFocused = d_time - d_widgetTimes[hash].focusedTime;
        info.sinceUnfocused = 0.0;
        info.keyPresses = d_keyPresses;
    }
    else {
        info.sinceFocused = 0.0;
        info.sinceUnfocused = d_time - d_widgetTimes[hash].unfocusedTime;
    }

    if (d_onClick == hash) { // Consume the onCLick
        d_onClick = 0;
        info.onClick = true;
    }

    if (d_onHover == hash) { // Consume the onHover
        d_onHover = 0;
        info.onHover = true;
    }

    if (d_onFocus == hash) { // Consume the onFocus
        d_onFocus = 0;
        info.onFocus = true;
    }

    return info;
}

DrawCommand& UIEngine::GetDrawCommand()
{
    assert(d_currentPanel);
    return d_currentPanel->mainCommand;
}

void UIEngine::OnEvent(Event& event)
{
    if (d_focused != 0 && !event.IsConsumed()) {
        if (auto e = event.As<KeyboardKeyTypedEvent>()) {
            d_keyPresses.push_back(e->Key());
            e->Consume();
        }
        else if (auto e = event.As<KeyboardButtonPressedEvent>()) {
            if (e->Key() == Keyboard::BACKSPACE) {
                d_keyPresses.push_back(Keyboard::BACKSPACE);
                e->Consume();
            }
        }
        else if (auto e = event.As<KeyboardButtonHeldEvent>()) {
            if (e->Key() == Keyboard::BACKSPACE) {
                d_keyPresses.push_back(Keyboard::BACKSPACE);
                e->Consume();
            }
        }
    }

    if (auto e = event.As<MouseButtonPressedEvent>()) {
        if (e->Button() == Mouse::LEFT) {
            d_mouseClicked = true;
            if (d_consumeMouseEvents) { e->Consume(); }
        }
    }
    else if (auto e = event.As<MouseButtonReleasedEvent>()) {
        if (e->Button() == Mouse::LEFT) {
            d_widgetTimes[d_clicked].unclickedTime = d_time;
            d_clicked = 0;
        }
    }
}

void UIEngine::OnUpdate(double dt)
{
    d_time += dt;
}

void UIEngine::StartFrame()
{
    d_panels.clear();
    d_currentPanel = nullptr;
}

void UIEngine::MouseClick()
{
    bool foundClicked = false;
    glm::vec2 mouse = d_window->GetMousePos();

    std::size_t moveToFront = 0;

    for (const auto& panelHash : Reversed(d_panelOrder)) {
        const auto& panel = d_panels[panelHash];

        for (const auto& quad : Reversed(panel.widgetRegions)) {
            std::size_t hash = quad.hash;
            auto hovered = InRegion(mouse, quad.region);
            auto clicked = hovered && d_mouseClicked;

            if (!foundClicked && ((d_clicked == hash) || clicked)) {
                foundClicked = true;
                moveToFront = panelHash;
                if (d_clicked != hash) {
                    d_widgetTimes[d_clicked].unclickedTime = d_time;
                    d_widgetTimes[hash].clickedTime = d_time;
                    d_clicked = hash;
                    d_onClick = hash;

                    // The newly clicked widget is now the focus
                    d_widgetTimes[d_focused].unfocusedTime = d_time;
                    d_widgetTimes[hash].focusedTime = d_time;
                    d_focused = hash;
                    d_onFocus = hash;
                }
            }
        }
    }

    // Clicked on something other than the UI, so lose focus
    if (d_mouseClicked && !foundClicked && d_focused > 0) {
        d_widgetTimes[d_focused].unfocusedTime = d_time;
        d_focused = 0;
    }

    if (moveToFront > 0) {
        auto toMove = std::find(d_panelOrder.begin(), d_panelOrder.end(), moveToFront);
        d_panelOrder.erase(toMove);
        d_panelOrder.push_back(moveToFront);
    }
}

void UIEngine::MouseHover()
{
    bool foundHovered = false;
    glm::vec2 mouse = d_window->GetMousePos();

    for (const auto& panelHash : Reversed(d_panelOrder)) {
        const auto& panel = d_panels[panelHash];

        for (const auto& quad : Reversed(panel.widgetRegions)) {
            std::size_t hash = quad.hash;
            auto hovered = InRegion(mouse, quad.region);
            
            if (!foundHovered && hovered) {
                foundHovered = true;
                if (d_hovered != hash) {
                    d_widgetTimes[d_hovered].unhoveredTime = d_time;
                    d_widgetTimes[hash].hoveredTime = d_time;
                    d_hovered = hash;
                    d_onHover = hash;
                }
            }
        }
    }

    // Hovered over something other than the UI, so lose hover
    if (!foundHovered && d_hovered > 0) {
        d_widgetTimes[d_hovered].unhoveredTime = d_time;
        d_hovered = 0;
    }

    d_consumeMouseEvents = foundHovered;
}

void UIEngine::EndFrame()
{
    assert(!d_currentPanel);

    MouseClick();
    MouseHover();

    d_keyPresses.clear();
    d_mouseClicked = false;

    Sprocket::RenderContext rc;
    rc.AlphaBlending(true);
    rc.FaceCulling(false);
    rc.DepthTesting(false);

    float w = (float)d_window->Width();
    float h = (float)d_window->Height();

    // This transformation makes the top left of the screen (0, 0) and the bottom
    // right be (width, height). It flips the y-axis since OpenGL treats the bottom
    // left as (0, 0).
    auto proj = glm::ortho(0.0f, w, h, 0.0f);
    d_shader.Bind();
    d_shader.LoadMat4("u_proj_matrix", proj);

    d_buffer.Bind();
    for (const auto& panelHash : d_panelOrder) {
        const auto& panel = d_panels[panelHash];

        ExecuteCommand(panel.mainCommand);
        for (const auto& cmd : panel.extraCommands) {
            ExecuteCommand(cmd);
        }
    }
    d_buffer.Unbind();
}

void UIEngine::StartPanel(const std::string& name, glm::vec4* region, PanelType type)
{
    assert(!d_currentPanel);
    std::size_t hash = std::hash<std::string>{}(name);

    auto it = std::find(d_panelOrder.begin(), d_panelOrder.end(), hash);
    if (it == d_panelOrder.end()) {
        d_panelOrder.push_back(hash);
    }
    
    auto& panel = d_panels[hash];
    panel.name = name;
    panel.hash = hash;
    panel.region = *region;
    d_currentPanel = &panel;

    if (type == PanelType::CLICKABLE || type == PanelType::DRAGGABLE) {
        auto info = Register(name, {0, 0, region->z, region->w});

        if (info.sinceClicked > 0 && type == PanelType::DRAGGABLE) {
            region->x += d_window->GetMouseOffset().x;
            region->y += d_window->GetMouseOffset().y;
            d_currentPanel->region = *region;
        }
    }
}

void UIEngine::EndPanel()
{
    assert(d_currentPanel);
    d_currentPanel = nullptr;
}

void UIEngine::SubmitDrawCommand(const DrawCommand& cmd)
{
    assert(d_currentPanel);
    d_currentPanel->extraCommands.emplace_back(cmd);
}

void UIEngine::ExecuteCommand(const DrawCommand& cmd)
{
    auto scissor = ScissorContext(d_window, cmd.region);
    if (cmd.texture) {
        cmd.texture->Bind(0);
    } else {
        d_white.Bind(0);
    }
    d_buffer.Draw(cmd.vertices, cmd.indices);
    
    if (cmd.font) {
        cmd.font->Bind(0);
        d_shader.LoadInt("texture_channels", 1);
        d_buffer.Draw(cmd.textVertices, cmd.textIndices);
    }
}

}