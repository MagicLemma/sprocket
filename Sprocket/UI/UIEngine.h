#pragma once
#include "KeyboardProxy.h"
#include "MouseProxy.h"
#include "Maths.h"

#include <unordered_map>
#include <cstddef>
#include <string>

namespace Sprocket {

struct QuadData
{
    std::string name;
    Maths::vec4 region;
};

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
// track of the currently clicked/hovered widgets. We only need to store
// information on the currently selected widgets, whereas we store a map
// for unselected widgets.
{
    KeyboardProxy* d_keyboard;
    MouseProxy* d_mouse;

    std::size_t d_hovered = 0;
    std::size_t d_clicked = 0;
        // Hashes of the currently hovered/clicked widgets.

    double d_hoveredTime = 0.0;
    double d_clickedTime = 0.0;
        // Times (in seconds) that the current widgets have been
        // hovered/selected.

    std::unordered_map<std::size_t, double> d_unhoveredTimes;
    std::unordered_map<std::size_t, double> d_unclickedTimes;
        // Hash -> time map keeping track of the last time each
        // widget was unselected. Used to calculate the unhovered
        // and unclicked times.

    double d_time = 0.0;
        // A steadily increasing timer used to set the unselected
        // times in the maps above.

    std::vector<QuadData> d_quads;
        // Stores a list of all regions registered during the frame,
        // these are processed at the end of the frame to update the
        // internal state of each widget.

    std::size_t d_onClick = 0;
    std::size_t d_onHover = 0;
        // Stores which widget has been clicked/hovered so that it can
        // be acted on next frame. These are consumed when retrieved.

public:
    UIEngine(KeyboardProxy* keyboard, MouseProxy* mouse);

    WidgetInfo RegisterWidget(const std::string& name,
                              const Maths::vec4& region);
        // Register a widget with the given name and region on the
        // screen. This tests if the widget is hovered and clicekd

    void StartFrame();
    void EndFrame();

    void OnUpdate(double dt);
};

}