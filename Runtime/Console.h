#include <Sprocket/UI/SimpleUI.h>

#include <glm/glm.hpp>

#include <deque>
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
    spkt::Window*  d_window;
    spkt::SimpleUI d_ui;

    std::string             d_commandLine;
    std::deque<ConsoleLine> d_consoleLines;

    void HandleCommand(std::string_view command);

public:
    Console(spkt::Window* window);

    void on_update(double dt);
    void on_event(spkt::event& event);
    void Draw();
};