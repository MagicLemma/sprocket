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

void Console::submit()
{
    print(d_commandLine);
    handle_command(d_commandLine);
    d_commandLine.clear();
}

void Console::handle_command(const std::string_view command)
{
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
        log("");
        return;
    }

    const std::string& directive = tokens[0];
    if (auto it = d_command_handlers.find(directive); it != d_command_handlers.end()) {
        it->second(*this, tokens);
    } else {
        error("Unknown command: '{}'", directive);
    }
}

void Console::clear_history()
{
    d_consoleLines.clear();
}

void Console::print(const std::string& line, const glm::vec4& colour)
{
    d_consoleLines.push_front({line, colour});
    while (d_consoleLines.size() > 100) {
        d_consoleLines.pop_back();
    }
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

void draw_console(Console& console, spkt::SimpleUI& ui, int width, int height)
{
    double W = 0.8 * width - 20;
    double H = height - 20;
    glm::vec4 mainRegion = {10, 10, W, H};

    ui.StartPanel("Main", &mainRegion, spkt::PanelType::UNCLICKABLE);

    double boxHeight = 50.0;
    ui.TextModifiable("Text", {10, H - 10 - boxHeight, W - 20, boxHeight}, &console.command_line());
    glm::vec2 region = {10, H - 10 - boxHeight - 50};
    float fontSize = 24.0f;
    for (const auto& command : console.history()) {
        ui.Text(command.text, fontSize, region, command.colour);
        region.y -= fontSize;
    }

    ui.EndPanel();
}