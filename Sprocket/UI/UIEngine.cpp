#include "UIEngine.h"
#include "MouseCodes.h"

namespace Sprocket {

UIEngine::UIEngine(KeyboardProxy* keyboard, MouseProxy* mouse)
    : d_keyboard(keyboard)
    , d_mouse(mouse)
{
}

WidgetInfo UIEngine::RegisterWidget(const std::string& name,
                                    const Maths::vec4& region)
{
    WidgetInfo info;

    std::size_t hash = std::hash<std::string>{}(name);
    auto hovered = d_mouse->InRegion(region.x, region.y, region.z, region.w);
    auto clicked = hovered && d_mouse->IsButtonClicked(Mouse::LEFT);

    if ((d_clicked == hash) || clicked) {
        d_clickedFlag = true;
        if (d_clicked != hash) {
            d_clicked = hash;
            d_clickedTime = 0.0;
            info.onClick = true;
        }
        info.clicked = d_clickedTime;
    }
    else {
        if (d_unclickedTimes.find(hash) == d_unclickedTimes.end()) {
            d_unclickedTimes[hash] = 0.0;
        }
        info.unclicked = d_time - d_unclickedTimes[hash];
    }
    
    if (hovered) {
        d_hoveredFlag = true;
        if (d_hovered != hash) {
            d_hovered = hash;
            d_hoveredTime = 0.0;
            info.onHover = true;
        }
        info.hovered = d_hoveredTime;
    }
    else {
        if (d_unhoveredTimes.find(hash) == d_unhoveredTimes.end()) {
            d_unhoveredTimes[hash] = 0.0;
        }
        info.unhovered = d_time - d_unhoveredTimes[hash];
    }

    return info;
}

void UIEngine::StartFrame()
{
    d_hoveredFlag = false;
    d_clickedFlag = false;
}

void UIEngine::EndFrame()
{
    if (d_hoveredFlag == false) {
        d_hoveredTime = 0.0;
        if (d_hovered > 0) {
            d_unhoveredTimes[d_hovered] = d_time;
            d_hovered = 0;
        }
    }

    if (d_clickedFlag == false) {
        d_clickedTime = 0.0;
        if (d_clicked > 0) {
            d_unclickedTimes[d_clicked] = d_time;
            d_hovered = 0;
        } 
    }
}

void UIEngine::OnUpdate(double dt)
{
    d_time += dt;
    d_clickedTime += dt;
    d_hoveredTime += dt;

    if (d_mouse->IsButtonReleased(Mouse::LEFT)) {
        d_clickedTime = 0.0;
        if (d_clicked > 0) {
            d_unclickedTimes[d_clicked] = d_time;
            d_clicked = 0;
        }
    }
}

}