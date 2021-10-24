#include "console.h"

#include <Sprocket/UI/simple_ui.h>

#include <sstream>
#include <vector>

namespace {

std::vector<std::string> tokenize(const std::string& command)
{
    std::vector<std::string> out;
    std::stringstream in;
    in << command;
    std::string token;
    while (std::getline(in, token, ' ')) {
        out.push_back(token);
    }
    return out;
}

}

namespace spkt {

void console::submit(const std::string& command)
{
    print(command);

    const std::vector<std::string> tokens = tokenize(command);
    if (tokens.empty()) {
        log("");
        return;
    }

    const std::string& directive = tokens[0];
    if (auto it = d_handlers.find(directive); it != d_handlers.end()) {
        it->second(*this, tokens);
    } else {
        error("Unknown command: '{}'", directive);
    }
}

void console::clear_history()
{
    d_lines.clear();
}

void console::print(const std::string& line, const glm::vec4& colour)
{
    d_lines.push_front({line, colour});
    while (d_lines.size() > 100) {
        d_lines.pop_back();
    }
}

void console::register_command(const std::string& command, const command_handler& handler)
{
    d_handlers.emplace(command, handler);
}

void console::deregister_command(const std::string& command)
{
    d_handlers.erase(command);
}

void draw_console(
    const console& console,
    std::string& command_line,
    spkt::simple_ui& ui,
    int width,
    int height)
{
    double W = 0.8 * width - 20;
    double H = height - 20;
    glm::vec4 mainRegion = {10, 10, W, H};

    ui.start_panel("Main", &mainRegion, spkt::panel_type::UNCLICKABLE);

    double boxHeight = 50.0;
    ui.text_modifiable("Text", {10, H - 10 - boxHeight, W - 20, boxHeight}, &command_line);
    glm::vec2 region = {10, H - 10 - boxHeight - 50};
    float fontSize = 24.0f;
    for (const auto& command : console.history()) {
        ui.text(command.text, fontSize, region, command.colour);
        region.y -= fontSize;
    }

    ui.end_panel();
}

}