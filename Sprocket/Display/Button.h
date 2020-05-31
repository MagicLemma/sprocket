#pragma once
#include "Widget.h"
#include "Quad.h"
#include "Maths.h"

namespace Sprocket {

struct ButtonAttributes
{
    float buttonWidth = 0.9f;
    float buttonHeight = 0.9f;
        // Width and height of button as percentage of the background size.

    float buttonWidthHovered = 0.9f;
    float buttonHeightHovered = 0.9f;
        // Width and height of button when the mouse is hovering over it
        // as percentage of the background size.

    float buttonWidthClicked = 0.9f;
    float buttonHeightClicked = 0.9f;
        // Width and height of button when clicked as percentage of the
        // background size.
};

using ButtonCallback = std::function<void()>;

class Button : public Widget
{
    Quad d_actual;

    Quad d_buttonNormal;
    Quad d_buttonHovered;
    Quad d_buttonClicked;

    bool d_hovered;
    bool d_clicked;

    ButtonCallback d_hoverCallback = [](){};
    ButtonCallback d_unhoverCallback = [](){};
    ButtonCallback d_clickCallback = [](){};
    ButtonCallback d_unclickCallback = [](){};

    void OnUpdateImpl(DisplayRenderer* renderer) override;
    void OnEventImpl(Event& event) override;

public:
    Button(float width,
           float height,
           float buttonScale = 0.5f,
           float buttonHoveredScale = 0.55f,
           float buttonClickedScale = 0.45f);

    Quad& ButtonNormal() { return d_buttonNormal; }
    Quad& ButtonHovered() { return d_buttonHovered; }
    Quad& ButtonClicked() { return d_buttonClicked; }

    const Quad& ButtonNormal() const { return d_buttonNormal; }
    const Quad& ButtonHovered() const { return d_buttonHovered; }
    const Quad& ButtonClicked() const { return d_buttonClicked; }

    void SetHoverCallback(ButtonCallback cb) { d_hoverCallback = cb; }
    void SetUnhoverCallback(ButtonCallback cb) { d_unhoverCallback = cb; }
    void SetClickCallback(ButtonCallback cb) { d_clickCallback = cb; }
    void SetUnclickCallback(ButtonCallback cb) { d_unclickCallback = cb; }
};

}