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
    Quad  d_background;
    Quad  d_backgroundHovered;
    Quad  d_backgroundClicked;
    Quad* d_activeBackground;

    Quad  d_button;
    Quad  d_buttonHovered;
    Quad  d_buttonClicked;
    Quad* d_activeButton;

    bool d_hovered;
    bool d_clicked;

    ButtonCallback d_hoverCallback = [](){};
    ButtonCallback d_unhoverCallback = [](){};
    ButtonCallback d_clickCallback = [](){};
    ButtonCallback d_unclickCallback = [](){};

    void updateImpl(Window* window) override;
    bool handleEventImpl(Window* window, const Event& event) override;
    void drawImpl(DisplayRenderer* renderer) const override;

public:
    Button(float width,
           float height,
           float buttonScale = 0.5f,
           float buttonHoveredScale = 0.5f,
           float buttonClickedScale = 0.5f);

    Quad& background() { return d_background; }
    Quad& backgroundHovered() { return d_backgroundHovered; }
    Quad& backgroundClicked() { return d_backgroundClicked; }

    Quad& button() { return d_button; }
    Quad& buttonHovered() { return d_buttonHovered; }
    Quad& buttonClicked() { return d_buttonClicked; }

    const Quad& background() const { return d_background; }
    const Quad& backgroundHovered() const { return d_backgroundHovered; }
    const Quad& backgroundClicked() const { return d_backgroundClicked; }

    const Quad& button() const { return d_button; }
    const Quad& buttonHovered() const { return d_buttonHovered; }
    const Quad& buttonClicked() const { return d_buttonClicked; }

    void setHoverCallback(ButtonCallback cb) { d_hoverCallback = cb; }
    void setUnhoverCallback(ButtonCallback cb) { d_unhoverCallback = cb; }
    void setClickCallback(ButtonCallback cb) { d_clickCallback = cb; }
    void setUnclickCallback(ButtonCallback cb) { d_unclickCallback = cb; }
};

}