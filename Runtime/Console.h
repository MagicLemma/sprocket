#include <Sprocket/UI/SimpleUI.h>

#include <glm/glm.hpp>

#include <deque>
#include <functional>
#include <ranges>
#include <string_view>

namespace spkt {
    class Window;
    class event;
}

struct ConsoleLine
{
    std::string text;
    glm::vec4 colour;
};

class Console
{
public:
    using command_handler = std::function<void(Console&, std::span<const std::string>)>;

private:
    std::string             d_commandLine;
    std::deque<ConsoleLine> d_consoleLines;

    std::unordered_map<std::string, command_handler> d_command_handlers;

    void handle_command(const std::string_view command);

public:

    void submit();

    void clear_history();

    void register_command(const std::string& command, const command_handler& handler);
    void deregister_command(const std::string& command);

    void print(const std::string& line, const glm::vec4& colour = {1.0, 1.0, 1.0, 1.0});
    void log(std::string_view format, auto&&... args);
    void error(std::string_view format, auto&&... args);

    const std::string& command_line() const { return d_commandLine; }
    std::string& command_line() { return d_commandLine; }

    const std::deque<ConsoleLine>& history() const { return d_consoleLines; }
};

template <typename... Args>
void Console::log(std::string_view format, Args&&... args)
{
    print(std::format(format, std::forward<Args>(args)...));
}

template <typename... Args>
void Console::error(std::string_view format, Args&&... args)
{
    print(std::format(format, std::forward<Args>(args)...), {1.0, 0.0, 0.0, 1.0});
}

void draw_console(Console& console, spkt::SimpleUI& ui, int width, int height);