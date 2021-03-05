#include "Console.h"
#include <Sprocket.h>
#include <filesystem>
#include <fmt/core.h>

namespace Sprocket {

const auto LIGHT_BLUE  = Sprocket::FromHex(0x25CCF7);
const auto CLEAR_BLUE  = Sprocket::FromHex(0x1B9CFC);
const auto GARDEN      = Sprocket::FromHex(0x55E6C1);
const auto SPACE_DARK  = Sprocket::FromHex(0x2C3A47);

Console::Console(Window* window)
    : d_window(window)
    , d_ui(window)
{
    SimpleUITheme theme;
    theme.backgroundColour = SPACE_DARK;
    theme.backgroundColour.w = 0.8f;
    theme.baseColour = CLEAR_BLUE;
    theme.hoveredColour = LIGHT_BLUE;
    theme.clickedColour = GARDEN;
    d_ui.SetTheme(theme);
}

void Console::OnUpdate(double dt)
{
    d_ui.OnUpdate(dt);
}

void Console::OnEvent(ev::Event& event)
{
    auto data = event.get_if<ev::KeyboardButtonPressed>();
    if (data && data->key == Keyboard::ENTER) {
        d_consoleLines.push_front({d_commandLine, glm::vec4{1.0, 1.0, 1.0, 1.0}});
        HandleCommand(d_commandLine);
        d_commandLine = "";
        event.consume();
    } else {
        d_ui.OnEvent(event);
    }
}

void Console::Draw()
{
    d_ui.StartFrame();

    double W = 0.8 * d_window->Width() - 20;
    double H = d_window->Height() - 20;
    glm::vec4 mainRegion = {10, 10, W, H};
    d_ui.StartPanel("Main", &mainRegion, PanelType::UNCLICKABLE);

    double boxHeight = 50.0;
    d_ui.TextModifiable("Text", {10, H - 10 - boxHeight, W - 20, boxHeight}, &d_commandLine);
    while (d_consoleLines.size() > 100) {
        d_consoleLines.pop_back();
    }
    glm::vec2 region = {10, H - 10 - boxHeight - 50};
    float fontSize = 24.0f;
    for (const auto& command : d_consoleLines) {
        d_ui.Text(command.text, fontSize, region, command.colour);
        region.y -= fontSize;
    }

    d_ui.EndPanel();
    d_ui.EndFrame();
}

void Console::HandleCommand(std::string_view command)
{
    if (command == "clear") {
        d_consoleLines.clear();
    }
    else if (command == "exit") {
        d_window->Close();
    }
    else if (command.substr(0, 5) == "echo ") {
        d_consoleLines.push_front({
            fmt::format(" > {}", command.substr(5)),
            glm::vec4{0.7, 0.7, 0.7, 1.0}
        });
    }
    else if (command.substr(0, 4) == "run ") {
        Sprocket::lua::LuaEngine engine;
        if (command.size() > 4) {  // Script name is at least a single character
            auto name = command.substr(4);
            auto script = fmt::format("Resources/Scripts/{}", name);
            if (std::filesystem::exists(script)) {
                engine.run_script(script);
            } else {
                d_consoleLines.push_front({
                    fmt::format(" > Could not find script '{}'", name),
                    glm::vec4{1.0, 0.0, 0.0, 1.0}
                });
            }
        }
    }
}

}