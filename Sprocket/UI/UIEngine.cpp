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

#include <functional>
#include <sstream>
#include <cassert>
#include <algorithm>

namespace Sprocket {

UIEngine::UIEngine(Window* window, KeyboardProxy* keyboard, MouseProxy* mouse)
    : d_window(window)
    , d_keyboard(keyboard)
    , d_mouse(mouse)
    , d_shader("Resources/Shaders/SimpleUI.vert",
               "Resources/Shaders/SimpleUI.frag")
    , d_font("Resources/Fonts/Coolvetica.ttf")
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

WidgetInfo UIEngine::Register(const std::string& name,
                              const Maths::vec4& region)
{
    assert(d_currentPanel);
    
    WidgetInfo info;
    info.quad = ApplyOffset(region);
    
    std::string prefixedName = d_currentPanel->name + "##" + name;
    std::size_t hash = std::hash<std::string>{}(prefixedName);
    d_panels[d_currentPanel->hash].widgetRegions.push_back({hash, info.quad});

    if (hash == d_clicked) {
        info.mouseDown = d_clickedTime;
    }

    info.sinceUnlicked = d_time - d_widgetTimes[hash].unclickedTime;
    info.sinceClicked = d_time - d_widgetTimes[hash].clickedTime;

    if (hash == d_hovered) {
        info.mouseOver = d_hoveredTime;
    }
    
    info.sinceUnhovered = d_time - d_widgetTimes[hash].unhoveredTime;
    info.sinceHovered = d_time - d_widgetTimes[hash].hoveredTime;

    if (d_onClick == hash) { // Consume the onCLick
        d_onClick = 0;
        info.onClick = true;
    }

    if (d_onHover == hash) { // Consume the onHover
        d_onHover = 0;
        info.onHover = true;
    }

    return info;
}

void UIEngine::OnEvent(Event& event)
{
}

void UIEngine::OnUpdate(double dt)
{
    d_mouse->ConsumeEvents(false);
    d_time += dt;
    d_clickedTime += dt;
    d_hoveredTime += dt;

    if (d_mouse->IsButtonReleased(Mouse::LEFT)) {
        d_clickedTime = 0.0;
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
                    d_clickedTime = 0.0;
                }
            }
            
            if (!foundHovered && hovered) {
                foundHovered = true;
                if (d_hovered != hash) {
                    d_widgetTimes[d_hovered].unhoveredTime = d_time;
                    d_widgetTimes[hash].hoveredTime = d_time;
                    d_hovered = hash;
                    d_onHover = hash;
                    d_hoveredTime = 0.0;
                }
            }
        }
    }

    if (foundHovered == false) {
        d_hoveredTime = 0.0;
        if (d_hovered > 0) {
            d_widgetTimes[d_hovered].unhoveredTime = d_time;
            d_hovered = 0;
        }
        d_mouse->ConsumeEvents(false);
    }
    else {
        d_mouse->ConsumeEvents(true);
    }

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
    d_shader.LoadUniformMat4("u_proj_matrix", proj);

    d_buffer.Bind();
    for (const auto& panelHash : d_panelOrder) {
        const auto& panel = d_panels[panelHash];
        d_shader.LoadUniformInt("texture_channels", 1);
        Texture::White().Bind();
        d_buffer.Draw(panel.quadVertices, panel.quadIndices);
        d_font.Bind();
        d_buffer.Draw(panel.textVertices, panel.textIndices);
        for (const auto& cmd : panel.extraCommands) {
            d_shader.LoadUniformInt("texture_channels", cmd.texture->GetChannels());
            cmd.texture->Bind();
            d_buffer.Draw(cmd.vertices, cmd.indices);
        }
    }
    d_buffer.Unbind();
}

bool UIEngine::StartPanel(
    const std::string& name,
    Maths::vec4* region,
    bool* active,
    bool* draggable,
    bool* clickable)
{
    assert(!d_currentPanel);
    assert(region != nullptr);
    assert(active != nullptr);

    if (*active) {
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

        if (*clickable) {
            auto info = Register(
                name,
                {0, 0, region->z, region->w}
            );

            if (info.mouseDown && *draggable) {
                region->x += d_mouse->GetMouseOffset().x;
                region->y += d_mouse->GetMouseOffset().y;
                d_currentPanel->region = *region;
            }
        }

    }

    return *active;
}

void UIEngine::EndPanel()
{
    assert(d_currentPanel);
    d_currentPanel = nullptr;
}

void UIEngine::DrawQuad(const Maths::vec4& colour,
                        const Maths::vec4& region)
{
    assert(d_currentPanel);
    auto copy = region;
    float x = copy.x;
    float y = copy.y;
    float width = copy.z;
    float height = copy.w;

    auto& cmd = d_panels[d_currentPanel->hash];

    auto col = colour;
    col.a = 1;

    std::size_t index = cmd.quadVertices.size();
    cmd.quadVertices.push_back({{x,         y},          col});
    cmd.quadVertices.push_back({{x + width, y},          col});
    cmd.quadVertices.push_back({{x,         y + height}, col});
    cmd.quadVertices.push_back({{x + width, y + height}, col});

    cmd.quadIndices.push_back(index + 0);
    cmd.quadIndices.push_back(index + 1);
    cmd.quadIndices.push_back(index + 2);
    cmd.quadIndices.push_back(index + 2);
    cmd.quadIndices.push_back(index + 1);
    cmd.quadIndices.push_back(index + 3);
}

void UIEngine::DrawText(
    const std::string& text,
    float size,
    const Maths::vec4& region,
    Alignment alignment)
{
    assert(d_currentPanel);

    auto copy = region;
    Maths::vec4 colour = {1.0, 1.0, 1.0, 1.0};

    Glyph first = d_font.GetGlyph(text.front(), size);
    float textWidth = d_font.TextWidth(text, size);

    Maths::vec2 pen{region.x, region.y};

    pen.y += (copy.w - first.height) / 2.0f;

    if (alignment == Alignment::LEFT) {
        pen.x += 5.0f;
    } else if (alignment == Alignment::RIGHT) {
        pen.x += region.z - 5.0f - textWidth;
    } else {
        pen.x += (copy.z - textWidth) / 2.0f;
    }

    pen.x -= first.offset.x;
    pen.y += first.offset.y;
    
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

        auto& cmd = d_panels[d_currentPanel->hash];

        unsigned int index = cmd.textVertices.size();
        cmd.textVertices.push_back({{xPos,         yPos},          colour, {x,     y    }});
        cmd.textVertices.push_back({{xPos + width, yPos},          colour, {x + w, y    }});
        cmd.textVertices.push_back({{xPos,         yPos + height}, colour, {x,     y + h}});
        cmd.textVertices.push_back({{xPos + width, yPos + height}, colour, {x + w, y + h}});

        cmd.textIndices.push_back(index + 0);
        cmd.textIndices.push_back(index + 1);
        cmd.textIndices.push_back(index + 2);
        cmd.textIndices.push_back(index + 2);
        cmd.textIndices.push_back(index + 1);
        cmd.textIndices.push_back(index + 3);
    }
}

void UIEngine::SubmitDrawCommand(const DrawCommand& cmd)
{
    assert(d_currentPanel);
    d_currentPanel->extraCommands.emplace_back(cmd);
}

}