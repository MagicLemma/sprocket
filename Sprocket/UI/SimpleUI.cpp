#include "SimpleUI.h"
#include "KeyboardProxy.h"
#include "MouseProxy.h"
#include "MouseCodes.h"
#include "KeyboardCodes.h"
#include "Log.h"
#include "Maths.h"
#include "RenderContext.h"
#include "BufferLayout.h"

#include <functional>
#include <sstream>
#include <cassert>
#include <algorithm>

namespace Sprocket {
namespace {

struct TextInfo
{
    float width = 0.0f;
    float height = 0.0f;
};

TextInfo GetTextInfo(Font& font, const std::string& text)
{
    TextInfo info;
    for (char c : text) {
        auto glyph = font.GetGlyph(c);
        info.width += glyph.advance.x;
        if (glyph.height > info.height) {
            info.height = (float)glyph.height;
        }
    }

    Glyph first = font.GetGlyph(text.front());
    info.width -= first.offset.x;

    Glyph last = font.GetGlyph(text.back());
    info.width += last.width;
    info.width += last.offset.x;
    info.width -= last.advance.x;

    return info;
}

template <typename T> T Interpolate(
    const WidgetInfo& info,
    const T& base,
    const T& hovered,
    const T& clicked)
{
    T ret = base;
    double interval = 0.1;
    
    if (info.mouseOver) {
        float r = std::min(info.mouseOver, interval) / (float)interval;
        ret = (1 - r) * ret + r * hovered;
    } else {
        float r = std::min(info.sinceUnhovered, interval) / (float)interval;
        ret = (1 - r) * hovered + r * ret;
    }

    if (info.mouseDown) {
        float r = std::min(info.mouseDown, interval) / (float)interval;
        ret = (1 - r) * ret + r * clicked;
    } else {
        float r = std::min(info.sinceUnlicked, interval) / (float)interval;
        ret = (1 - r) * clicked + r * ret;
    }

    return ret;
}

}

SimpleUI::SimpleUI(Window* window)
    : d_window(window)
    , d_shader("Resources/Shaders/SimpleUI.vert",
               "Resources/Shaders/SimpleUI.frag")
    , d_font("Resources/Fonts/Coolvetica.ttf", 36.0f)
{
    d_keyboard.ConsumeAll(false);

    BufferLayout layout(sizeof(BufferVertex));
    layout.AddAttribute(DataType::FLOAT, 2);
    layout.AddAttribute(DataType::FLOAT, 4);
    layout.AddAttribute(DataType::FLOAT, 2);
    d_buffer.SetBufferLayout(layout);
}

Maths::vec4 SimpleUI::ApplyOffset(const Maths::vec4& region)
{
    if (d_currentPanel) {
        Maths::vec4 quad = region;
        quad.x += d_currentPanel->region.x;
        quad.y += d_currentPanel->region.y;
        return quad;
    }
    return region;
}

std::string SimpleUI::MangleName(const std::string& name)
{
    return d_currentPanel->name + "##" + name;
}

void SimpleUI::OnEvent(Event& event)
{
    d_keyboard.OnEvent(event);
    d_mouse.OnEvent(event);
}

void SimpleUI::OnUpdate(double dt)
{
    d_mouse.OnUpdate();
    d_time += dt;
    d_clickedTime += dt;
    d_hoveredTime += dt;

    if (d_mouse.IsButtonReleased(Mouse::LEFT)) {
        d_clickedTime = 0.0;
        if (d_clicked > 0) {
            d_widgetTimes[d_clicked].unclickedTime = d_time;
            d_clicked = 0;
        }
    }
}

void SimpleUI::StartFrame()
{
    d_panels.clear();
    d_currentPanel = nullptr;
}

void SimpleUI::EndFrame()
{
    assert(!d_currentPanel);

    bool foundHovered = false;
    bool foundClicked = false;

    std::size_t moveToFront = 0;

    for (auto it = d_panelOrder.rbegin(); it != d_panelOrder.rend(); ++it) {
        const auto& panel = d_panels[*it];
        const auto& quads = panel.widgetRegions;
        for (std::size_t i = quads.size(); i != 0;) {
            --i;
            const auto& quad = quads[i];
            std::size_t hash = quad.hash;
            auto hovered = d_mouse.InRegion(quad.region.x, quad.region.y, quad.region.z, quad.region.w);
            auto clicked = hovered && d_mouse.IsButtonClicked(Mouse::LEFT);

            if (!foundClicked && ((d_clicked == hash) || clicked)) {
                foundClicked = true;
                moveToFront = *it;
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
    }

    Sprocket::RenderContext rc;
    rc.AlphaBlending(true);
    rc.FaceCulling(false);
    rc.DepthTesting(false);

    float w = (float)d_window->Width();
    float h = (float)d_window->Height();
    auto proj = Maths::Ortho(0, w, h, 0);
    d_shader.Bind();
    d_shader.LoadUniform("u_proj_matrix", proj);

    d_buffer.Bind();
    for (auto it = d_panelOrder.begin(); it != d_panelOrder.end(); ++it) {
        const auto& cmd = d_panels[*it];
        Texture::White().Bind();
        d_buffer.Draw(cmd.quadVertices, cmd.quadIndices);
        d_font.Bind();
        d_buffer.Draw(cmd.textVertices, cmd.textIndices);
    }
    d_buffer.Unbind();

    if (moveToFront > 0) {
        auto toMove = std::find(d_panelOrder.begin(), d_panelOrder.end(), moveToFront);
        d_panelOrder.erase(toMove);
        d_panelOrder.push_back(moveToFront);
    }
}

bool SimpleUI::StartPanel(
    const std::string& name,
    Maths::vec4* region,
    bool* active,
    bool* draggable)
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
        
        auto info = RegisterWidget(name, *region);

        if (info.mouseDown && *draggable) {
            region->x += d_mouse.GetMouseOffset().x;
            region->y += d_mouse.GetMouseOffset().y;
        }

        DrawQuad(d_theme.backgroundColour * 0.7f, *region);
    }

    return *active;
}

void SimpleUI::EndPanel()
{
    assert(d_currentPanel);
    d_currentPanel = nullptr;
}

void SimpleUI::DrawQuad(const Maths::vec4& colour,
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

void SimpleUI::DrawText(
    const std::string& text,
    const Maths::vec4& region)
{
    assert(d_currentPanel);

    auto copy = region;
    Maths::vec4 colour = {1.0, 1.0, 1.0, 1.0};

    Glyph first = d_font.GetGlyph(text.front());
    auto textInfo = GetTextInfo(d_font, text);

    Maths::vec2 pen = {
        region.x + (copy.z - textInfo.width) / 2.0f,
        region.y + (copy.w - first.height) / 2.0f
    };

    pen.x -= first.offset.x;
    pen.y += first.offset.y;
    
    for (std::size_t i = 0; i != text.size(); ++i) {
        auto glyph = d_font.GetGlyph(text[i]);

        if (i > 0) {
            pen.x += d_font.GetKerning(text[i-1], text[i]);
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

void SimpleUI::Quad(const Maths::vec4& colour, const Maths::vec4& quad)
{
    assert(d_currentPanel);
    auto copy = ApplyOffset(quad);
    DrawQuad(colour, copy);
}

void SimpleUI::Text(const std::string& text, const Maths::vec4& quad)
{
    assert(d_currentPanel);
    auto copy = ApplyOffset(quad);
    DrawText(text, copy);
}

bool SimpleUI::Button(const std::string& name,
                      const Maths::vec4& region)
{
    assert(d_currentPanel);
    Maths::vec4 copy = ApplyOffset(region);
    auto info = RegisterWidget(MangleName(name), copy);

    Maths::vec4 hoveredRegion = copy;
    hoveredRegion.x -= 10.0f;
    hoveredRegion.z += 20.0f;

    Maths::vec4 clickedRegion = copy;
    clickedRegion.x += 10.0f;
    clickedRegion.z -= 20.0f;

    Maths::vec4 colour = Interpolate(info, d_theme.baseColour, d_theme.hoveredColour, d_theme.clickedColour);
    Maths::vec4 shape = Interpolate(info, copy, hoveredRegion, clickedRegion);
    
    DrawQuad(colour, shape);
    DrawText(name, copy);

    return info.onClick;
}

bool SimpleUI::Checkbox(const std::string& name,
                        const Maths::vec4& region,
                        bool* value)
{
    assert(d_currentPanel);
    Maths::vec4 copy = ApplyOffset(region);
    auto info = RegisterWidget(MangleName(name), copy);

    auto unselected = Interpolate(info, d_theme.backgroundColour, d_theme.backgroundColour*1.1f, d_theme.clickedColour);
    auto selected = Interpolate(info, d_theme.baseColour, d_theme.hoveredColour, d_theme.clickedColour);;

    float r = std::min(info.sinceClicked, 0.1) / 0.1f;

    Maths::vec4 colour;
    if (*value) {
        colour = r * selected + (1 - r) * unselected;
    } else {
        colour = r * unselected + (1 - r) * selected;
    }

    if (info.onClick) {
        *value = !(*value);
    }

    DrawQuad(colour, copy);
    DrawText(name, copy);

    return *value; 
}

void SimpleUI::Slider(const std::string& name,
                      const Maths::vec4& region,
                      float* value, float min, float max)
{
    assert(d_currentPanel);
    Maths::vec4 copy = ApplyOffset(region);
    auto info = RegisterWidget(MangleName(name), copy);

    float x = copy.x;
    float y = copy.y;
    float width = copy.z;
    float height = copy.w;

    Maths::vec4 leftColour = Interpolate(info, d_theme.baseColour, d_theme.hoveredColour, d_theme.clickedColour);
    Maths::vec4 rightColour = d_theme.backgroundColour;
    
    float ratio = (*value - min) / (max - min);
    DrawQuad(leftColour, {x, y, ratio * width, height});
    DrawQuad(rightColour, {x + ratio * width, y, (1 - ratio) * width, height});
    DrawText(name + ": " + Maths::ToString(*value, 0), copy);

    if (info.mouseDown) {
        auto mouse = d_mouse.GetMousePos();
        Maths::Clamp(mouse.x, x, x + width);
        float r = (mouse.x - x) / width;
        *value = (1 - r) * min + r * max;
    }    
}

void SimpleUI::Dragger(const std::string& name,
                       const Maths::vec4& region,
                       float* value, float speed)
{
    assert(d_currentPanel);
    Maths::vec4 copy = ApplyOffset(region);
    auto info = RegisterWidget(MangleName(name), copy);

    Maths::vec4 colour = Interpolate(info, d_theme.baseColour, d_theme.hoveredColour, d_theme.clickedColour);
    
    DrawQuad(colour, copy);
    DrawText(name + ": " + Maths::ToString(*value, 0), copy);

    if (info.mouseDown) {
        *value += d_mouse.GetMouseOffset().x * speed;
    }    
}

WidgetInfo SimpleUI::RegisterWidget(const std::string& name,
                                    const Maths::vec4& region)
{
    assert(d_currentPanel);
    WidgetInfo info;
    std::size_t hash = std::hash<std::string>{}(name);
    d_panels[d_currentPanel->hash].widgetRegions.push_back({hash, region});

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


}