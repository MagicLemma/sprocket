#include <Sprocket.h>
#include <deque>

namespace Sprocket {

class Console
{
    Window*  d_window;
    SimpleUI d_ui;

    std::string             d_commandLine;
    std::deque<std::string> d_consoleLines;

    void HandleCommand(const std::string& command);

public:
    Console(Window* window);

    void OnUpdate(double dt);
    void OnEvent(Event& event);
    void Draw();
};

}