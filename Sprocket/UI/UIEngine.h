#pragma once
#include "KeyboardProxy.h"
#include "MouseProxy.h"
#include "Maths.h"

#include <unordered_map>
#include <cstddef>
#include <string>

namespace Sprocket {

struct WidgetInfo
// When registering a new widget with the UIEngine, the callers gets
// this struct back. It contains, in seconds, the amount of time that
// the current widget has been (un)hovered and (un)clicked, among
// other pieces of info. The caller can then use this information to
// decide how the widget should be rendered.
{
    double hovered = 0.0;
    double unhovered = 0.0;

    double clicked = 0.0;
    double unclicked = 0.0;
    
    bool onClick = false;
    bool onHover = false;
};

class UIEngine
// Intended to be used as a component in an IMGUI class. Used to keep
// track of the currently clicked/hovered widgets.
{
    KeyboardProxy* d_keyboard;
    MouseProxy* d_mouse;

    std::size_t d_hovered = 0;
    double d_hoveredTime = 0.0;
    bool d_hoveredFlag = false;
    std::unordered_map<std::size_t, double> d_unhoveredTimes;

    std::size_t d_clicked = 0;
    double d_clickedTime = 0.0;
    bool d_clickedFlag = false;
    std::unordered_map<std::size_t, double> d_unclickedTimes;

    double d_time = 0.0;

public:
    UIEngine(KeyboardProxy* keyboard, MouseProxy* mouse);

    WidgetInfo RegisterWidget(const std::string& name,
                              const Maths::vec4& region);

    void StartFrame();
    void EndFrame();

    void OnUpdate(double dt);
};

}