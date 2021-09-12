#pragma once
#include <Sprocket/UI/SimpleUI.h>

#include <glm/glm.hpp>

#include <deque>
#include <functional>
#include <span>
#include <string_view>
#include <string>

namespace spkt {

struct console_line
{
    std::string text;
    glm::vec4 colour;
};

class console
{
public:
    using command_handler = std::function<void(console&, std::span<const std::string>)>;

private:
    std::unordered_map<std::string, command_handler> d_handlers;
    std::deque<console_line>                         d_history;

public:
    void register_command(const std::string& command, const command_handler& handler);
    void deregister_command(const std::string& command);

    void submit(const std::string& command);

    [[nodiscard]] const std::deque<console_line>& history() const { return d_history; }
    void clear_history();

    void print(const std::string& line, const glm::vec4& colour = {1.0, 1.0, 1.0, 1.0});
    void log(std::string_view format, auto&&... args);
    void error(std::string_view format, auto&&... args);
};

template <typename... Args>
void console::log(std::string_view format, Args&&... args)
{
    print(std::format(format, std::forward<Args>(args)...));
}

template <typename... Args>
void console::error(std::string_view format, Args&&... args)
{
    print(std::format(format, std::forward<Args>(args)...), {1.0, 0.0, 0.0, 1.0});
}

void draw_console(
    const console& console,
    std::string& command_line,
    spkt::SimpleUI& ui,
    int width,
    int height
);

}