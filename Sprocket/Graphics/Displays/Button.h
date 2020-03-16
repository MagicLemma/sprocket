#pragma once
#include "UiComponent.h"
#include "Quad.h"
#include "Maths.h"

namespace Sprocket {

struct ButtonAttributes
{
    Maths::vec3 backgroundColour = {0.0f, 0.0f, 0.0f};
    Maths::vec3 buttonColour = {1.0f, 1.0f, 1.0f};

    Maths::vec3 backgroundColourHovered = {0.0f, 0.0f, 0.0f};
    Maths::vec3 buttonColourHovered = {1.0f, 1.0f, 1.0f};

    Maths::vec3 backgroundColourClicked = {0.0f, 0.0f, 0.0f};
    Maths::vec3 buttonColourClicked = {1.0f, 1.0f, 1.0f};

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

class Button : public UiComponent
{
    ButtonAttributes d_attributes;

    Quad d_background;
    Quad d_button;

    bool d_hovered;
    bool d_clicked;

    ButtonCallback d_hoverCallback = [](){};
    ButtonCallback d_unhoverCallback = [](){};
    ButtonCallback d_clickCallback = [](){};
    ButtonCallback d_unclickCallback = [](){};

public:
    Button(const Maths::vec2& topLeft,
           float width,
           float height,
           const ButtonAttributes& attrs = {});

    Quad background() const { return d_background; }
    Quad button() const { return d_button; }

    ButtonAttributes attributes() const { return d_attributes; }

    void update(Window* window) override;
    void handleEvent(Window* window, const Event& event) override;

    void setHoverCallback(ButtonCallback cb) { d_hoverCallback = cb; }
    void setUnhoverCallback(ButtonCallback cb) { d_unhoverCallback = cb; }
    void setClickCallback(ButtonCallback cb) { d_clickCallback = cb; }
    void setUnclickCallback(ButtonCallback cb) { d_unclickCallback = cb; }
};

}