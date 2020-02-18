#include "Log.h"
#include "Window.h"
#include "Events/Event.h"

int main(int argc, char* argv[])
{
    Sprocket::Log::init();
    SPKT_LOG_CORE_INFO("Version {}.{}.{}", 0, 0, 1);

    Sprocket::Window window;

    while(window.running())
    {
        window.clear();
        window.onUpdate();
    }


    return 0;
}