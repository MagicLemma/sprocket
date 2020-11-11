#include <Sprocket.h>

namespace Sprocket {

class Console
{
    SimpleUI d_ui;
    std::string d_commandLine;

public:
    Console(Window* window);

    void OnUpdate(double dt);
    void OnEvent(Event& event);
    void Draw();

};

}