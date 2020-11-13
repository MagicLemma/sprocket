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

#include <functional>
#include <sstream>
#include <cassert>
#include <algorithm>

#include <glad/glad.h>

namespace Sprocket {
namespace {

std::vector<unsigned char> GetWhiteData()
{
    return {0xff, 0xff, 0xff, 0xff};
}

}

UIEngine::UIEngine(Window* window, KeyboardProxy* keyboard, MouseProxy* mouse)
    : d_window(window)
    , d_keyboard(keyboard)
    , d_mouse(mouse)
    , d_shader("Resources/Shaders/SimpleUI.vert",
               "Resources/Shaders/SimpleUI.frag")
    , d_font("Resources/Fonts/Coolvetica.ttf")
    , d_white(1, 1, GetWhiteData().data())
{
    BufferLayout layout(sizeof(BufferVertex));
    layout.AddAttribute(DataType::FLOAT, 2);
    layout.AddAttribute(DataType::FLOAT, 4);
    layout.AddAttribute(DataType::FLOAT, 2);
    d_buffer.SetBufferLayout(layout);
}

Maths::vec4 UIEngine::ApplyOffset(const Maths::vec4& region)
{
    if (d_currentPanel) {
        Maths::vec4 quad = region;
        quad.x += d_currentPanel->region.x;
        quad.y += d_currentPanel->region.y;
        return quad;
    }
    return region;
}

WidgetInfo UIEngine::Register(const std::string& name, const Maths::vec4& region)
{
    assert(d_currentPanel);
    
    WidgetInfo info;
    info.quad = ApplyOffset(region);
    
    std::string prefixedName = d_currentPanel->name + "##" + name;
    std::size_t hash = std::hash<std::string>{}(prefixedName);
    d_panels[d_currentPanel->hash].widgetRegions.push_back({hash, info.quad});

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
}

void UIEngine::OnUpdate(double dt)
{
    d_mouse->ConsumeEvents(false);
    d_time += dt;

    if (d_mouse->IsButtonReleased(Mouse::LEFT)) {
        // Let go of mouse click, so lose clicked
        if (d_clicked > 0) {
            d_widgetTimes[d_clicked].unclickedTime = d_time;
            d_clicked = 0;
        }
    }
}

void UIEngine::StartFrame()
{
    d_panels.clear();
    d_currentPanel = nullptr;
}

void UIEngine::EndFrame()
{
    assert(!d_currentPanel);

    bool foundHovered = false;
    bool foundClicked = false;

    std::size_t moveToFront = 0;

    d_keyPresses.clear();

    for (const auto& panelHash : Reversed(d_panelOrder)) {
        const auto& panel = d_panels[panelHash];

        for (const auto& quad : Reversed(panel.widgetRegions)) {
            std::size_t hash = quad.hash;
            auto hovered = d_mouse->InRegion(quad.region.x, quad.region.y, quad.region.z, quad.region.w);
            auto clicked = hovered && d_mouse->IsButtonClicked(Mouse::LEFT);

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

    if (d_mouse->IsButtonClicked(Mouse::LEFT) && !foundClicked) {
        // Clicked on something other than the UI, so lose focus
        if (d_focused > 0) {
            d_widgetTimes[d_focused].unfocusedTime = d_time;
            d_focused = 0;
        }
    }

    if (!foundHovered) {
        // Hovered over something other than the UI, so lose hover
        if (d_hovered > 0) {
            d_widgetTimes[d_hovered].unhoveredTime = d_time;
            d_hovered = 0;
        }
    }

    d_mouse->ConsumeEvents(foundHovered);

    if (moveToFront > 0) {
        auto toMove = std::find(d_panelOrder.begin(), d_panelOrder.end(), moveToFront);
        d_panelOrder.erase(toMove);
        d_panelOrder.push_back(moveToFront);
    }

    Sprocket::RenderContext rc;
    rc.AlphaBlending(true);
    rc.FaceCulling(false);
    rc.DepthTesting(false);

    float w = (float)d_window->Width();
    float h = (float)d_window->Height();
    auto proj = Maths::Ortho(0, w, h, 0);
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

bool UIEngine::StartPanel(
    const std::string& name,
    Maths::vec4* region,
    bool active,
    bool draggable,
    bool clickable)
{
    assert(!d_currentPanel);

    if (active) {
        std::size_t hash = std::hash<std::string>{}(name);

        auto it = std::find(d_panelOrder.begin(), d_panelOrder.end(), hash);
        if (it == d_panelOrder.end()) {
            d_panelOrder.push_back(hash);
        }
        
        auto& panel = d_panels[hash];
        panel.name = name;
        panel.hash = hash;
        panel.region = *region;
        panel.mainCommand.texture = &d_white;
        panel.mainCommand.font = &d_font;
        d_currentPanel = &panel;

        if (clickable) {
            auto info = Register(
                name,
                {0, 0, region->z, region->w}
            );

            if (info.sinceClicked > 0 && draggable) {
                region->x += d_mouse->GetMouseOffset().x;
                region->y += d_mouse->GetMouseOffset().y;
                d_currentPanel->region = *region;
            }
        }

    }

    return active;
}

void UIEngine::EndPanel()
{
    assert(d_currentPanel);
    d_currentPanel = nullptr;
}

void UIEngine::DrawQuad(const Maths::vec4& colour,
                        const Maths::vec4& region,
                        DrawCommand* cmd)
{
    assert(d_currentPanel);
    auto copy = region;
    float x = copy.x;
    float y = copy.y;
    float width = copy.z;
    float height = copy.w;

    DrawCommand* target = cmd;
    if (target == nullptr) {
        target = &d_panels[d_currentPanel->hash].mainCommand;
    }

    std::size_t index = target->vertices.size();
    target->vertices.push_back({{x,         y},          colour});
    target->vertices.push_back({{x + width, y},          colour});
    target->vertices.push_back({{x,         y + height}, colour});
    target->vertices.push_back({{x + width, y + height}, colour});

    target->indices.push_back(index + 0);
    target->indices.push_back(index + 1);
    target->indices.push_back(index + 2);
    target->indices.push_back(index + 2);
    target->indices.push_back(index + 1);
    target->indices.push_back(index + 3);
}

void UIEngine::DrawText(
    const std::string& text,
    float size,
    const Maths::vec4& region,
    Alignment alignment,
    const Maths::vec4& colour,
    DrawCommand* cmd)
{
    assert(d_currentPanel);
    if (text.size() == 0) { return; }

    auto copy = region;

    float textWidth = d_font.TextWidth(text, size);

    Maths::vec2 pen{region.x, region.y};

    if (alignment == Alignment::LEFT) {
        pen.x += 5.0f;
        pen.y += size;
    } else if (alignment == Alignment::RIGHT) {
        pen.x += region.z - 5.0f;
        pen.y += size;
    } else {
        Glyph first = d_font.GetGlyph(text.front(), size);
        pen.y += (copy.w - first.height) / 2.0f;
        pen.x += (copy.z - textWidth) / 2.0f;
        pen.x -= first.offset.x;
        pen.y += first.offset.y;
    }

    
    for (std::size_t i = 0; i != text.size(); ++i) {
        auto glyph = d_font.GetGlyph(text[i], size);

        if (i > 0) {
            pen.x += d_font.GetKerning(text[i-1], text[i], size);
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

        DrawCommand* target = cmd;
        if (target == nullptr) {
            target = &d_panels[d_currentPanel->hash].mainCommand;
        }

        u32 index = target->textVertices.size();
        target->textVertices.push_back({{xPos,         yPos},          colour, {x,     y    }});
        target->textVertices.push_back({{xPos + width, yPos},          colour, {x + w, y    }});
        target->textVertices.push_back({{xPos,         yPos + height}, colour, {x,     y + h}});
        target->textVertices.push_back({{xPos + width, yPos + height}, colour, {x + w, y + h}});

        target->textIndices.push_back(index + 0);
        target->textIndices.push_back(index + 1);
        target->textIndices.push_back(index + 2);
        target->textIndices.push_back(index + 2);
        target->textIndices.push_back(index + 1);
        target->textIndices.push_back(index + 3);
    }
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
        d_buffer.Draw(cmd.vertices, cmd.indices);
    }
    if (cmd.font) {
        cmd.font->Bind(0);
        d_shader.LoadInt("texture_channels", 1);
        d_buffer.Draw(cmd.textVertices, cmd.textIndices);
    }
}

}