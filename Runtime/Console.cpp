#include "Console.h"

namespace Sprocket {

const auto LIGHT_BLUE  = Sprocket::FromHex(0x25CCF7);
const auto CLEAR_BLUE  = Sprocket::FromHex(0x1B9CFC);
const auto GARDEN      = Sprocket::FromHex(0x55E6C1);
const auto SPACE_DARK  = Sprocket::FromHex(0x2C3A47);

Console::Console(Window* window)
    : d_ui(window)
{
    SimpleUITheme theme;
    theme.backgroundColour = SPACE_DARK;
    theme.baseColour = CLEAR_BLUE;
    theme.hoveredColour = LIGHT_BLUE;
    theme.clickedColour = GARDEN;
    d_ui.SetTheme(theme);
}

void Console::OnUpdate(double dt)
{
    d_ui.OnUpdate(dt);

    d_ui.StartFrame();

    Maths::vec4 mainRegion = {0, 0, 300, 300};
    bool active = true;
    d_ui.StartPanel("Main", &mainRegion, &active, &active, &active);

    d_ui.Button("Button1", {10, 10, 20, 20});
    d_ui.Button("Button2", {10, 30, 20, 20});
    d_ui.TextModifiable("Text", {10, 60, 200, 100}, &d_commandLine);

    d_ui.EndPanel();
}

void Console::OnEvent(Event& event)
{
    d_ui.OnEvent(event);
}

void Console::Draw()
{
    d_ui.EndFrame();
}

}