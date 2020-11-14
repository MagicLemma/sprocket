#include <Sprocket.h>
#include <deque>

namespace Sprocket {

struct ConsoleLine
{
    std::string text;
    Maths::vec4 colour;
};

class Console
{
    Window*  d_window;
    SimpleUI d_ui;

    std::string             d_commandLine;
    std::deque<ConsoleLine> d_consoleLines;

    void HandleCommand(const std::string& command);

public:
    Console(Window* window);

    void OnUpdate(double dt);
    void OnEvent(Event& event);
    void Draw();
};

}