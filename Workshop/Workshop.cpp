#include <iostream>

#include <Sprocket.h>

int main(int argc, char* argv[])
{
    Sprocket::begin(argc, argv);
    Sprocket::Log::init();
    SPKT_LOG_WARN("LOGGING IS WORKING YEAHHHHHH");
    return 0;
}