#include <Sprocket/UI/SimpleUI.h>

#include <glm/glm.hpp>

#include <deque>
#include <functional>
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
    spkt::Window*  d_window;
    spkt::SimpleUI d_ui;

    std::string             d_commandLine;
    std::deque<ConsoleLine> d_consoleLines;

    std::unordered_map<std::string, command_handler> d_command_handlers;

    void handle_command(const std::string_view command);

public:
    Console(spkt::Window* window);

    void on_update(double dt);
    void on_event(spkt::event& event);
    void draw();

    void submit();

    void clear_history();

    void register_command(const std::string& command, const command_handler& handler);
    void deregister_command(const std::string& command);

    void print(const std::string& line, const glm::vec4& colour = {1.0, 1.0, 1.0, 1.0});
    void log(std::string_view format, auto&&... args);
    void error(std::string_view format, auto&&... args);
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