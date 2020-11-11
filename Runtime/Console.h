#include <Sprocket.h>

namespace Sprocket {

class Console
{
    SimpleUI d_ui;

public:
    Console(Window* window);

    void OnUpdate(double dt);
    void OnEvent(Event& event);
    void Draw();

};

}