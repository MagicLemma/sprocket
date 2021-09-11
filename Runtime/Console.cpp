#include "Console.h"

#include <Sprocket/Core/Events.h>
#include <Sprocket/Core/Window.h>
#include <Sprocket/Scripting/LuaScript.h>
#include <Sprocket/UI/SimpleUI.h>
#include <Sprocket/Utility/Colour.h>
#include <Sprocket/Utility/KeyboardCodes.h>

#include <filesystem>
#include <format>
#include <ranges>

const auto LIGHT_BLUE  = spkt::from_hex(0x25CCF7);
const auto CLEAR_BLUE  = spkt::from_hex(0x1B9CFC);
const auto GARDEN      = spkt::from_hex(0x55E6C1);
const auto SPACE_DARK  = spkt::from_hex(0x2C3A47);

Console::Console(spkt::Window* window)
    : d_window(window)
    , d_ui(window)
{
    spkt::SimpleUITheme theme;
    theme.backgroundColour = SPACE_DARK;
    theme.backgroundColour.w = 0.8f;
    theme.baseColour = CLEAR_BLUE;
    theme.hoveredColour = LIGHT_BLUE;
    theme.clickedColour = GARDEN;
    d_ui.SetTheme(theme);
}

void Console::on_update(double dt)
{
    d_ui.on_update(dt);
}

void Console::on_event(spkt::event& event)
{
    auto data = event.get_if<spkt::keyboard_pressed_event>();
    if (data && data->key == spkt::Keyboard::ENTER) {
        d_consoleLines.push_front({d_commandLine, glm::vec4{1.0, 1.0, 1.0, 1.0}});
        HandleCommand(d_commandLine);
        d_commandLine = "";
        event.consume();
    } else {
        d_ui.on_event(event);
    }
}

void Console::Draw()
{
    d_ui.StartFrame();

    double W = 0.8 * d_window->Width() - 20;
    double H = d_window->Height() - 20;
    glm::vec4 mainRegion = {10, 10, W, H};
    d_ui.StartPanel("Main", &mainRegion, spkt::PanelType::UNCLICKABLE);

    double boxHeight = 50.0;
    d_ui.TextModifiable("Text", {10, H - 10 - boxHeight, W - 20, boxHeight}, &d_commandLine);
    if (d_consoleLines.size() > 100) {
        d_consoleLines.resize(100);
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

void Console::HandleCommand(const std::string_view command)
{
    if (command == "clear") {
        d_consoleLines.clear();
        return;
    }
    if (command == "exit") {
        d_window->Close();
        return;
    }
    if (command.substr(0, 5) == "echo ") {
        log_line(
            std::format(" > {}", command.substr(5)),
            glm::vec4{0.7, 0.7, 0.7, 1.0}
        );
        return;
    }
    if (command.substr(0, 4) == "run ") {
        if (command.size() > 4) {  // Script name is at least a single character
            auto name = command.substr(4);
            auto script_file = std::format("Resources/Scripts/{}", name);
            if (std::filesystem::exists(script_file)) {
                spkt::lua::Script script(script_file);
            } else {
                log_line(
                    std::format(" > Could not find script '{}'", name),
                    glm::vec4{1.0, 0.0, 0.0, 1.0}
                );
            }
        }
        return;
    }

    const std::vector<std::string> tokens = std::invoke([&] {
        std::vector<std::string> out;
        std::stringstream in;
        in << command;
        std::string token;
        while (std::getline(in, token, ' ')) {
            out.push_back(token);
        }
        return out;
    });

    if (tokens.empty()) {
        log_line("", {1.0, 1.0, 1.0, 1.0}); // Empty line
        return;
    }

    const std::string& directive = tokens[0];
    auto it = d_command_handlers.find(directive);
    if (it != d_command_handlers.end()) {
        it->second(*this, {std::next(tokens.begin()), tokens.end()});
    } else {
        log_line(
            std::format("Unknown command: '{}'", directive),
            {1.0, 0.0, 0.0, 1.0}
        );
    }
}

void Console::clear_history()
{
    d_consoleLines.clear();
}

void Console::log_line(const std::string& line, const glm::vec4& colour)
{
    d_consoleLines.push_front({line, colour});
}

void Console::register_command(const std::string& command, const command_handler& handler)
{
    d_command_handlers.emplace(command, handler);
}

void Console::deregister_command(const std::string& command)
{
    if (auto it = d_command_handlers.find(command); it != d_command_handlers.end()) {
        d_command_handlers.erase(it);
    }
}