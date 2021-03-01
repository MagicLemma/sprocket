#include <Sprocket.h>
#include <deque>
#include <string_view>

namespace Sprocket {

struct ConsoleLine
{
    std::string text;
    glm::vec4 colour;
};

class Console
{
    Window*  d_window;
    SimpleUI d_ui;

    std::string             d_commandLine;
    std::deque<ConsoleLine> d_consoleLines;

    void HandleCommand(std::string_view command);

public:
    Console(Window* window);

    void OnUpdate(double dt);
    void OnEvent(Event& event);
    void Draw();
};

}